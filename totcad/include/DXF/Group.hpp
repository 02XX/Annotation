#pragma once

#include <cstddef>
#include <optional>
#include <string>

namespace totcad::dxf
{

struct Group
{
    int code{0};
    std::string value;
    std::size_t line{0};

    std::optional<int> toInt() const noexcept;
    std::optional<double> toDouble() const noexcept;
};

} // namespace totcad::dxf
