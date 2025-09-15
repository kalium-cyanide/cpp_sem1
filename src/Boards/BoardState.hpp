#pragma once

#include "../Figures/IFigure.hpp"

struct BoardState
{
    matrix<IFigure*> figures_m;
    position_t size;
};