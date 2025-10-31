#pragma once

#include "utility.hpp"

struct Move
{
    position_t from;
    position_t to;

    enum special_moves
    {
        EMPTY,
        LONG_CASTLING,
        SHORT_CASTLING
    };

    special_moves special = EMPTY;
};