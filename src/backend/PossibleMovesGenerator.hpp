#pragma once

#include <map>
#include <set>

#include "Boards/IBoard.hpp"
#include "Figures/IFigure.hpp"
#include "utility.hpp"

using possible_moves_table = std::map<string_t, std::set<position_t>>;

class PossibleMoveGenerator {
public:
    static std::set<position_t> generatePossiblePosition(IBoard *board,
                                                         IFigure *figure) {
        auto possiblePosition = std::set<position_t>();
        auto size = board->getBoardState()->size;

        for (int fromI = 0; fromI < size.x; fromI++)
            for (int fromJ = 0; fromJ < size.y; fromJ++) {
                for (int i = 0; i < size.x; i++)
                    for (int j = 0; j < size.y; j++) {
                        position_t from(fromI + 1, fromJ + 1), to(i + 1, j + 1);

                        if (board->isPossibleSet(to, figure) and
                            figure->isPossibleMove({from, to})) {
                            auto offset = to - from;
                            possiblePosition.insert(offset);
                        }
                    }
            }

        return possiblePosition;
    }
};
