#include "DXF/Group.hpp"

#include <charconv>
#include <cerrno>
#include <cstdlib>
#include <limits>
#include <string_view>

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

} // namespace

std::optional<int> Group::toInt() const noexcept
{
    const std::string_view text = trim(value);
    int result = 0;
    const auto converted = std::from_chars(text.data(), text.data() + text.size(), result);
    if (converted.ec != std::errc{} || converted.ptr != text.data() + text.size())
        return std::nullopt;
    return result;
}

std::optional<double> Group::toDouble() const noexcept
{
    const std::string text(trim(value));
    if (text.empty())
        return std::nullopt;

    char *end = nullptr;
    errno = 0;
    const double result = std::strtod(text.c_str(), &end);
    if (errno == ERANGE || end != text.c_str() + text.size())
        return std::nullopt;
    return result;
}

} // namespace totcad::dxf
