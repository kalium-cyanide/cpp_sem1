#pragma once

#include "IFigure.hpp"

#include <vector>

class Knight : public IFigure
{
    color_t color_m;

  public:
    Knight(color_t color) : color_m(color) {}

    color_t getColor() { return color_m; }

    bool isPossibleMove(const Move& move)
    {
        position_t offset = move.to - move.from;

        if (std::abs(offset.x) == 2 and std::abs(offset.y) == 1) { return true; }
        if (std::abs(offset.x) == 1 and std::abs(offset.y) == 2) { return true; }

        return false;
    }
};
