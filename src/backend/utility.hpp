#pragma once
#include "Vector2.hpp"
#include <string>
#include <vector>
#include <cstdint>

using string_t = std::string;

using position_t = Vector2<int>;

using color_t = std::uint8_t;

namespace chess {
    inline constexpr color_t WHITE = 0;
    inline constexpr color_t BLACK = 1;
}

template <class t> using matrix = std::vector<std::vector<t>>;
template <class t> using array_t = std::vector<t>;