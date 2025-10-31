#pragma once

#include "Boards/IBoard.hpp"
#include "Figures/IFigure.hpp"
#include "PossibleMovesGenerator.hpp"
#include "utility.hpp"
#include <map>
#include <set>

using possible_moves_table = std::map<string_t, std::set<position_t>>;

struct DataFabric
{
    static possible_moves_table* createPossibleMoveTable(IBoard* board, array_t<IFigure*>& figures)
    {
        auto result = new possible_moves_table();

        for (auto i : figures)
        {
            auto&& type = i->getType();
            result->insert({type, PossibleMoveGenerator::generatePossiblePosition(board, i)});
        }

        return result;
    }
};