#pragma once

#include "IFigure.hpp"

#include <vector>

class Queen : public IFigure
{
    color_t color_m;

  public:
    Queen(color_t color) : color_m(color)
    {}

    color_t getColor() override
    {
        return color_m;
    }

    void setColor(color_t color) override
    {
        color_m = color;
    }

    bool isPossibleMove(const Move& move) override
    {
        position_t offset = move.to - move.from;

        if (offset.y == offset.x) { return true; }
        if (offset.y == -offset.x) { return true; }

        if (std::abs(offset.x) > 0 and offset.y == 0) { return true; }
        if (offset.x == 0 and std::abs(offset.y) > 0) { return true; }

        return false;
    }

    string_t getType() override
    {
        return "queen";
    }
};
