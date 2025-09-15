#pragma once

#include "../Move.hpp"
#include "../utility.hpp"

struct IPlayer
{
    virtual Move doMove() = 0;
    virtual string_t getName() = 0;
    virtual color_t getColor() = 0;
};