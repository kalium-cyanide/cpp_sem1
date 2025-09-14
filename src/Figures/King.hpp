#pragma once

#include "IFigure.hpp"
#include <cmath>
#include <vector>

class King : public IFigure
{
    color_t color_m;

  public:
    King(color_t color) : color_m(color) {}

    color_t getColor() { return color_m; }

    bool isPossibleMove(const Move& move)
    {
        position_t offset = move.to - move.from;

        if (offset == position_t(0, 0)) { return false; }

        if (abs(offset.x) > 1 and abs(offset.y) > 1) { return false; }

        return true;
    }
};
