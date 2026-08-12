#include "DXF/DXFDocument.hpp"

#include <algorithm>
#include <cctype>

namespace totcad::dxf {

namespace {

bool sameName(std::string_view left, std::string_view right) noexcept
{
    return left.size() == right.size()
        && std::equal(left.begin(), left.end(), right.begin(), [](char a, char b) {
               return std::toupper(static_cast<unsigned char>(a))
                   == std::toupper(static_cast<unsigned char>(b));
           });
}

} // namespace

const Group *Record::find(int code) const noexcept
{
    const auto found = std::find_if(groups.begin(), groups.end(), [code](const Group &group) {
        return group.code == code;
    });
    return found == groups.end() ? nullptr : &*found;
}

std::optional<std::string> Record::stringValue(int code) const
{
    const Group *group = find(code);
    return group ? std::optional<std::string>(group->value) : std::nullopt;
}

std::optional<int> Record::intValue(int code) const noexcept
{
    const Group *group = find(code);
    return group ? group->toInt() : std::nullopt;
}

std::optional<double> Record::doubleValue(int code) const noexcept
{
    const Group *group = find(code);
    return group ? group->toDouble() : std::nullopt;
}

const Table *DXFDocument::findTable(std::string_view name) const noexcept
{
    const auto found = std::find_if(tables.begin(), tables.end(), [&](const Table &table) {
        return sameName(table.name, name);
    });
    return found == tables.end() ? nullptr : &*found;
}

} // namespace totcad::dxf
