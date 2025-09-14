#pragma once

#include "../Move.hpp"
#include "../utility.hpp"

struct IBoard;

struct IFigure
{
    virtual color_t getColor() = 0;
    virtual bool isPossibleMove(const Move& move) = 0;
};
