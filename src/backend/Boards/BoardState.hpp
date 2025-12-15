#pragma once

#include <concepts>
#include <cstdio>
#include <type_traits>
#include <vector>

#include "../Figures/IFigure.hpp"
#include "../utility.hpp"

struct BoardState {
    position_t size;
    std::vector<std::vector<IFigure *>> figures_m;

    color_t player_to_move;
};
