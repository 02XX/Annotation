#pragma once

#include "DXF/DXFDocument.hpp"

#include <filesystem>
#include <istream>
#include <string>

namespace totcad::dxf
{

class Reader final
{
  public:
    bool read(std::istream &input, DXFDocument &document, std::string *error = nullptr) const;
    bool readFile(const std::filesystem::path &path,
                  DXFDocument &document,
                  std::string *error = nullptr) const;
};

} // namespace totcad::dxf
