#include "DXF/Reader.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <utility>
#include <vector>

namespace totcad::dxf {

namespace {

std::string_view trim(std::string_view value) noexcept
{
    constexpr std::string_view spaces{" \t\r\n\f\v"};
    const std::size_t first = value.find_first_not_of(spaces);
    if (first == std::string_view::npos)
        return {};
    return value.substr(first, value.find_last_not_of(spaces) - first + 1);
}

std::string trimmed(std::string_view value)
{
    return std::string(trim(value));
}

bool sameName(std::string_view left, std::string_view right) noexcept
{
    left = trim(left);
    right = trim(right);
    return left.size() == right.size()
        && std::equal(left.begin(), left.end(), right.begin(), [](char a, char b) {
               return std::toupper(static_cast<unsigned char>(a))
                   == std::toupper(static_cast<unsigned char>(b));
           });
}

bool isMarker(const Group &group, std::string_view value) noexcept
{
    return group.code == 0 && sameName(group.value, value);
}

void setError(std::string *error, std::size_t line, std::string message)
{
    if (!error)
        return;
    if (line != 0)
        *error = "line " + std::to_string(line) + ": " + std::move(message);
    else
        *error = std::move(message);
}

bool readGroups(std::istream &input, std::vector<Group> &groups, std::string *error)
{
    std::string codeText;
    std::string value;
    std::size_t line = 0;

    while (std::getline(input, codeText)) {
        ++line;
        if (!codeText.empty() && codeText.back() == '\r')
            codeText.pop_back();
        if (line == 1 && codeText.compare(0, 3, "\xEF\xBB\xBF") == 0)
            codeText.erase(0, 3);

        if (!std::getline(input, value)) {
            setError(error, line, "group code is missing its value line");
            return false;
        }
        ++line;
        if (!value.empty() && value.back() == '\r')
            value.pop_back();

        Group group{0, std::move(value), line - 1};
        Group codeGroup{0, std::move(codeText), line - 1};
        const std::optional<int> code = codeGroup.toInt();
        if (!code || *code < 0 || *code > 1071) {
            setError(error, line - 1, "invalid DXF group code");
            return false;
        }
        group.code = *code;
        groups.push_back(std::move(group));
    }

    if (!input.eof()) {
        setError(error, line, "failed while reading DXF stream");
        return false;
    }
    return true;
}

std::vector<Record> readRecords(const std::vector<Group> &groups,
                                std::size_t begin,
                                std::size_t end)
{
    std::vector<Record> records;
    std::size_t index = begin;
    while (index < end) {
        if (groups[index].code != 0) {
            ++index;
            continue;
        }

        Record record;
        record.type = trimmed(groups[index].value);
        ++index;
        while (index < end && groups[index].code != 0) {
            record.groups.push_back(groups[index]);
            ++index;
        }
        records.push_back(std::move(record));
    }
    return records;
}

void readTables(const std::vector<Group> &groups,
                std::size_t begin,
                std::size_t end,
                DXFDocument &document)
{
    std::size_t index = begin;
    while (index < end) {
        if (!isMarker(groups[index], "TABLE")) {
            ++index;
            continue;
        }

        const std::size_t tableBegin = index++;
        std::size_t tableEnd = index;
        while (tableEnd < end && !isMarker(groups[tableEnd], "ENDTAB"))
            ++tableEnd;

        Table table;
        std::size_t firstRecord = tableEnd;
        for (std::size_t field = tableBegin + 1; field < tableEnd; ++field) {
            if (groups[field].code == 2 && table.name.empty())
                table.name = trimmed(groups[field].value);
            if (groups[field].code == 0) {
                firstRecord = field;
                break;
            }
        }
        table.records = readRecords(groups, firstRecord, tableEnd);
        document.tables.push_back(std::move(table));
        index = tableEnd < end ? tableEnd + 1 : tableEnd;
    }
}

void readBlocks(const std::vector<Group> &groups,
                std::size_t begin,
                std::size_t end,
                DXFDocument &document)
{
    std::size_t index = begin;
    while (index < end) {
        if (!isMarker(groups[index], "BLOCK")) {
            ++index;
            continue;
        }

        const std::size_t blockBegin = index;
        std::size_t blockEnd = index + 1;
        while (blockEnd < end && !isMarker(groups[blockEnd], "ENDBLK"))
            ++blockEnd;

        const std::vector<Record> records = readRecords(groups, blockBegin, blockEnd);
        if (!records.empty()) {
            Block block;
            block.definition = records.front();
            block.entities.assign(records.begin() + 1, records.end());
            document.blocks.push_back(std::move(block));
        }
        index = blockEnd < end ? blockEnd + 1 : blockEnd;
    }
}

bool buildDocument(const std::vector<Group> &groups, DXFDocument &document, std::string *error)
{
    bool foundSection = false;
    std::size_t index = 0;
    while (index < groups.size()) {
        if (isMarker(groups[index], "EOF"))
            break;
        if (!isMarker(groups[index], "SECTION")) {
            ++index;
            continue;
        }

        foundSection = true;
        if (index + 1 >= groups.size() || groups[index + 1].code != 2) {
            setError(error, groups[index].line, "SECTION has no group 2 name");
            return false;
        }

        const std::string sectionName = trimmed(groups[index + 1].value);
        const std::size_t sectionBegin = index + 2;
        std::size_t sectionEnd = sectionBegin;
        while (sectionEnd < groups.size() && !isMarker(groups[sectionEnd], "ENDSEC"))
            ++sectionEnd;
        if (sectionEnd == groups.size()) {
            setError(error, groups[index].line, "SECTION has no ENDSEC");
            return false;
        }

        if (sameName(sectionName, "HEADER")) {
            document.header.assign(groups.begin() + static_cast<std::ptrdiff_t>(sectionBegin),
                                   groups.begin() + static_cast<std::ptrdiff_t>(sectionEnd));
        } else if (sameName(sectionName, "TABLES")) {
            readTables(groups, sectionBegin, sectionEnd, document);
        } else if (sameName(sectionName, "BLOCKS")) {
            readBlocks(groups, sectionBegin, sectionEnd, document);
        } else if (sameName(sectionName, "ENTITIES")) {
            document.entities = readRecords(groups, sectionBegin, sectionEnd);
        }
        index = sectionEnd + 1;
    }

    if (!foundSection) {
        setError(error, 0, "DXF contains no SECTION");
        return false;
    }
    return true;
}

} // namespace

bool Reader::read(std::istream &input, DXFDocument &document, std::string *error) const
{
    std::vector<Group> groups;
    if (!readGroups(input, groups, error))
        return false;

    DXFDocument parsed;
    if (!buildDocument(groups, parsed, error))
        return false;

    document = std::move(parsed);
    if (error)
        error->clear();
    return true;
}

bool Reader::readFile(const std::filesystem::path &path,
                      DXFDocument &document,
                      std::string *error) const
{
    std::ifstream input(path, std::ios::binary);
    if (!input) {
        setError(error, 0, "cannot open DXF file: " + path.string());
        return false;
    }
    return read(input, document, error);
}

} // namespace totcad::dxf
