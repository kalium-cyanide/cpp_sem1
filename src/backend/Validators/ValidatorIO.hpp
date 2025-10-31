#pragma once

#include "../Board.hpp"
#include "../Figure.hpp"
#include "../Players.hpp"
#include "../PossibleMovesGenerator.hpp"
#include "../utility.hpp"

struct ValidatorInput
{
    possible_moves_table* possible_moves;
    IBoard* board;
    IPlayer* player;
    Move move;
    array_t<std::tuple<IPlayer*, IFigure*, Move>>* move_history;
};

struct ValidatorOutput
{
    array_t<string_t> errors;
    bool result = true;
};