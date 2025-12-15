#pragma once

#include "../Move.hpp"
#include "../utility.hpp"

struct IFigure {
    virtual color_t getColor() = 0;
    virtual void setColor(color_t color) = 0;

    virtual bool isPossibleMove(const Move &move) = 0;
    virtual string_t getType() = 0;
};
