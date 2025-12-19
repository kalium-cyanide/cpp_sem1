#pragma once

#include "utility.hpp"

struct Move {
    position_t from;
    position_t to;

    enum special_moves {
        EMPTY,
        LONG_CASTLING,
        SHORT_CASTLING,
        QUEEN_PROMOTION,
        KNIGHT_PROMOTION,
        ROOK_PROMOTION,
        BISHOP_PROMOTION,
        EN_PASSANT,
    };

    special_moves special = EMPTY;
};