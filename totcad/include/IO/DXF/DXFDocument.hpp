#pragma once

#include "IO/DXF/Group.hpp"

#include <optional>
#include <string>
#include <string_view>
#include <vector>

namespace totcad::dxf
{

struct Record
{
    std::string type;
    std::vector<Group> groups;

    const Group *find(int code) const noexcept;
    std::optional<std::string> stringValue(int code) const;
    std::optional<int> intValue(int code) const noexcept;
    std::optional<double> doubleValue(int code) const noexcept;
};

struct Table
{
    std::string name;
    std::vector<Record> records;
};

struct Block
{
    Record definition;
    std::vector<Record> entities;
};

struct DXFDocument
{
    std::vector<Group> header;
    std::vector<Table> tables;
    std::vector<Block> blocks;
    std::vector<Record> entities;

    const Table *findTable(std::string_view name) const noexcept;
};

} // namespace totcad::dxf
