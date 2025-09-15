#pragma once

#include "IFigure.hpp"

#include <vector>

class Bishop : public IFigure
{
    color_t color_m;

  public:
    Bishop(color_t color) : color_m(color)
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

        if (offset.y == offset.x)
        {
            return true;
        }
        if (offset.y == -offset.x)
        {
            return true;
        }

        return false;
    }

    string_t getType() override
    {
        return "bishop";
    }
};
