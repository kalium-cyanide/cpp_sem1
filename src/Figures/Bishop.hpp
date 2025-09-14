#pragma once

#include "IFigure.hpp"

#include <vector>

class Bishop : public IFigure
{
    color_t color_m;

  public:
    Bishop(color_t color) : color_m(color) {}

    color_t getColor() { return color_m; }

    bool isPossibleMove(const Move& move)
    {
        position_t offset = move.to - move.from;

        if (offset.y == offset.x) { return true; }
        if (offset.y == -offset.x) { return true; }

        return false;
    }
};
