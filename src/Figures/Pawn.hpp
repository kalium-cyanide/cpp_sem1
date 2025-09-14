#pragma once

#include "IFigure.hpp"

#include <vector>

class Pawn : public IFigure
{
    color_t color_m;

  public:
    Pawn(color_t color) : color_m(color) {}

    color_t getColor() { return color_m; }

    bool isPossibleMove(const Move& move)
    {
        position_t offset = move.to - move.from;

        if (offset.y == 1 or offset.y == 2) { return true; }

        return false;
    }
};
