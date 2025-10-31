#pragma once

#include "../Figures/IFigure.hpp"
#include "../utility.hpp"
#include <vector>

struct BoardState
{
    position_t size;
    std::vector<std::vector<IFigure*>> figures_m;

    color_t player_to_move;
};