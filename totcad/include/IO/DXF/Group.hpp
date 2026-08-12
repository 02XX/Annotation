#pragma once

#include <cstddef>
#include <optional>
#include <string>

namespace totcad::dxf
{

// DXF 的最小数据单元：组码、原始值以及组码所在行。
struct Group
{
    int code{0};
    std::string value;
    std::size_t line{0};

    std::optional<int> toInt() const noexcept;
    std::optional<double> toDouble() const noexcept;
};

} // namespace totcad::dxf
