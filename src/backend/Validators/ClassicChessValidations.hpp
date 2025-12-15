#include <functional>
#include <map>
#include <set>

#include "../DataFabric.hpp"
#include "Validator.hpp"
#include "ValidatorIO.hpp"

struct ClassicChessValidations {
    static void FigureExistenceValidation(const ValidatorInput &in,
                                          ValidatorOutput &out,
                                          std::function<void()> next) {
        auto &board = in.board;
        auto &move = in.move;
        auto boardState = board->getBoardState();
        auto &figures = boardState->figures_m;

        auto movedFigure = figures[move.from.x - 1][move.from.y - 1];

        if (movedFigure == nullptr) {
            out.result = false;
            out.errors.push_back("has no figure on cell");
            return;
        }

        next();
    }

    static void ColorValidation(const ValidatorInput &in, ValidatorOutput &out,
                                std::function<void()> next) {
        auto &board = in.board;
        auto &player = in.player;
        auto &move = in.move;
        auto boardState = board->getBoardState();
        auto &figures = boardState->figures_m;

        auto movedFigure = figures[move.from.x - 1][move.from.y - 1];
        auto attackedFigure = figures[move.to.x - 1][move.to.y - 1];

        if (movedFigure->getColor() != player->getColor()) {
            out.result = false;
            out.errors.push_back("wrong color move");
        }
        if (attackedFigure != nullptr &&
            attackedFigure->getColor() == player->getColor()) {
            out.result = false;
            out.errors.push_back("u can take self figures");
        }

        next();
    };

    static void PathClearValidation(const ValidatorInput &in,
                                    ValidatorOutput &out,
                                    std::function<void()> next) {
        auto &board = in.board;
        auto &player = in.player;
        auto &move = in.move;
        auto boardState = board->getBoardState();
        auto &figures = boardState->figures_m;
        auto movedFigure = figures[move.from.x - 1][move.from.y - 1];

        auto ox = move.to.x - move.from.x, oy = move.to.y - move.from.y;
        auto d = position_t(ox == 0 ? (0) : ox / std::abs(ox),
                            oy == 0 ? (0) : oy / std::abs(oy));

        std::cout << d << std::endl;
        std::cout << move.from << ' ' << move.to << std::endl;

        if (dynamic_cast<Knight *>(movedFigure) != nullptr) {
            next();
            return;
        }

        for (position_t i = move.from + d; i != move.to; i += d) {
            if (i.x < 1 || i.x > boardState->size.x || i.y < 1 ||
                i.y > boardState->size.y) {
                break;
            }

            if (board->getFigure(i) != nullptr) {
                out.result = false;
                out.errors.push_back("path is not empty");
                next();
                return;
            }
        }

        next();
    }

    static void CastlingValidation(const ValidatorInput &in, ValidatorOutput &out,
                                   std::function<void()> next) {
        auto &board = in.board;
        auto &move = in.move;
        auto &player = in.player;
        auto boardState = board->getBoardState();
        auto &size = boardState->size;

        if (move.special != Move::LONG_CASTLING and
            move.special != Move::SHORT_CASTLING) {
            next();
            return;
        }

        auto isLongCastling = move.special == Move::LONG_CASTLING;

        auto kingPos = position_t{5, (player->getColor() == 0) ? (1) : (8)};
        auto rookPos = position_t{isLongCastling ? (1) : (8),
                                  (player->getColor() == 0) ? (1) : (8)};

        auto king = board->getFigure(kingPos);
        auto rook = board->getFigure(rookPos);

        if (dynamic_cast<King *>(king) == nullptr or
            dynamic_cast<Rook *>(rook) == nullptr) {
            out.result = false;
            out.errors.push_back("castling wrong");
            return;
        }

        for (int i = std::min(kingPos.x, rookPos.x) + 1;
             i < std::max(kingPos.x, rookPos.x); i++) {
            std::cout << position_t(i, kingPos.y) << '\n';
            if (board->getFigure({i, kingPos.y}) != nullptr) {
                out.result = false;
                out.errors.push_back("path is not empty");
                return;
            }
        }
    }

    static void BoardBorderValidation(const ValidatorInput &in,
                                      ValidatorOutput &out,
                                      std::function<void()> next) {
        std::cout << "BoardBorderValidation\n";

        auto &board = in.board;
        auto &move = in.move;
        auto boardState = board->getBoardState();
        auto &size = boardState->size;

        if (move.special != Move::EMPTY) {
            next();
            return;
        }

        if (move.to.x < 1 || move.to.x > size.x) {
            out.result = false;
            out.errors.push_back("move is out of board");
            return;
        }
        if (move.to.y < 1 || move.to.y > size.y) {
            out.result = false;
            out.errors.push_back("move is out of board");
            return;
        }

        next();
    };

    static void FigureMovePatternValidation(const ValidatorInput &in,
                                            ValidatorOutput &out,
                                            std::function<void()> next) {
        std::cout << "FigureMovePatternValidation\n";

        auto &possible_moves = in.possible_moves;
        auto &board = in.board;
        auto &move = in.move;
        auto &player = in.player;
        auto boardState = board->getBoardState();
        auto &figures = boardState->figures_m;

        auto movedFigure = figures[move.from.x - 1][move.from.y - 1];
        auto &currentSet = possible_moves->at(movedFigure->getType());

        auto offset =
                position_t(move.to - move.from) *
                (player->getColor() == 0 ? position_t(1, 1) : position_t(-1, -1));
        std::cout << "offset: " << offset << '\n';
        if (currentSet.find(offset) == currentSet.end()) {
            out.result = false;
            out.errors.push_back("move doesn't match figure pattern");
            return;
        }

        next();
    };

    static void FirstPawnMoveValidation(const ValidatorInput &in,
                                        ValidatorOutput &out,
                                        std::function<void()> next) {
        std::cout << "FirstPawnMoveValidation\n";

        auto &board = in.board;
        auto &player = in.player;
        auto &move = in.move;
        auto boardState = board->getBoardState();
        auto &figures = boardState->figures_m;

        auto movedFigure = figures[move.from.x - 1][move.from.y - 1];
        auto pawnLine = (player->getColor() == 0) ? 2 : 7;

        if (dynamic_cast<Pawn *>(movedFigure) == nullptr) {
            next();
            return;
        }

        if (std::abs(move.to.y - move.from.y) == 2 and move.from.y != pawnLine) {
            out.result = false;
            out.errors.push_back(
                    "pawn can move 2 cells forward only from start position");
        }

        next();
    };

    static void PawnAttackValidation(const ValidatorInput &in,
                                     ValidatorOutput &out,
                                     std::function<void()> next) {
        std::cout << "PawnAttackValidation\n";

        auto &board = in.board;
        auto &move = in.move;
        auto boardState = board->getBoardState();
        auto &figures = boardState->figures_m;

        auto movedFigure = board->getFigure(move.from);
        auto attackedFigure = board->getFigure(move.to);

        if (dynamic_cast<Pawn *>(movedFigure) == nullptr) {
            next();
            return;
        }

        if (std::abs(move.to.x - move.from.x) == 1 and
            std::abs(move.to.y - move.from.y) == 1) {
            if (attackedFigure != nullptr) {
                return;
            }
        } else if (move.to.x == move.from.x) {
            if (attackedFigure == nullptr) {
                next();
                return;
            }
        }

        out.result = false;
        out.errors.push_back("invalid pawn move");
        next();
    };

    static void PawnEnPassantValidation(const ValidatorInput &in,
                                        ValidatorOutput &out,
                                        std::function<void()> next) {
        auto &board = in.board;
        auto &player = in.player;
        auto &move = in.move;
        auto &move_history = in.move_history;
        auto boardState = board->getBoardState();
        auto &figures = boardState->figures_m;

        auto movedFigure = board->getFigure(move.from);
        auto attackedFigure = board->getFigure(move.to);

        if (dynamic_cast<Pawn *>(movedFigure) == nullptr) {
            next();
            return;
        }

        if (move.to.x == move.from.x) {
            next();
            return;
        }

        if (move_history->empty()) {
            out.result = false;
            out.errors.push_back("no previous move for en passant");
            next();
            return;
        }

        auto [lastPlayer, lastFigure, lastMove] = move_history->back();

        bool enPassantCondition = lastFigure->getType() == "pawn" &&
                                  std::abs(lastMove.to.y - lastMove.from.y) == 2 &&
                                  lastMove.to.y == move.from.y &&
                                  lastMove.to.x == move.to.x;

        if (std::abs(move.to.x - move.from.x) == 1 && attackedFigure == nullptr &&
            enPassantCondition) {
            next();
            return;
        }

        out.result = false;
        out.errors.push_back("wrong en passant");
        next();
    };

    static void KingSafetyValidation(const ValidatorInput &in,
                                     ValidatorOutput &out,
                                     std::function<void()> next) {
        auto &board = in.board;
        auto &move = in.move;
        auto &player = in.player;

        IFigure *movedFig = board->getFigure(move.from);
        IFigure *targetFig = board->getFigure(move.to);

        board->setFigure(move.to, movedFig);
        board->removeFigure(move.from);

        position_t kingPos = {-1, -1};
        auto boardState = board->getBoardState();

        for (int x = 1; x <= boardState->size.x; ++x) {
            for (int y = 1; y <= boardState->size.y; ++y) {
                auto fig = boardState->figures_m[x - 1][y - 1];

                if (fig != nullptr && fig->getType() == "king" &&
                    fig->getColor() == player->getColor()) {
                    kingPos = {x, y};
                }
            }
        }

        bool kingInCheck = false;
        if (kingPos.x != -1) {
            if (isUnderAttack(board, kingPos, player->getColor())) {
                kingInCheck = true;
            }
        }

        board->setFigure(move.from, movedFig);
        board->setFigure(move.to, targetFig);

        if (kingInCheck) {
            out.result = false;
            out.errors.push_back("move leaves king in check");
            next();
            return;
        }

        next();
    }

    static bool isUnderAttack(IBoard *board, position_t pos,
                              color_t defenderColor) {
        auto boardState = board->getBoardState();
        auto &figures = boardState->figures_m;
        color_t attackerColor = (defenderColor == 0) ? 1 : 0;

        for (int x = 1; x <= boardState->size.x; ++x) {
            for (int y = 1; y <= boardState->size.y; ++y) {
                auto fig = figures[x - 1][y - 1];
                if (fig == nullptr || fig->getColor() != attackerColor) continue;

                Move attackMove = {{x, y}, pos, Move::EMPTY};

                if (fig->getType() == "pawn") {
                    int direction = (attackerColor == 0) ? 1 : -1;
                    if (pos.y - y == direction && std::abs(pos.x - x) == 1) {
                        return true;
                    }
                    continue;
                }

                if (!fig->isPossibleMove(attackMove)) continue;

                string_t type = fig->getType();
                if (type == "knight" || type == "king") {
                    return true;
                }

                bool pathClear = true;
                auto ox = pos.x - x;
                auto oy = pos.y - y;

                auto d = position_t(ox == 0 ? 0 : ox / std::abs(ox),
                                    oy == 0 ? 0 : oy / std::abs(oy));

                for (position_t i = position_t(x, y) + d; i != pos; i += d) {
                    if (board->getFigure(i) != nullptr) {
                        pathClear = false;
                        break;
                    }
                }

                if (pathClear) return true;
            }
        }
        return false;
    }
};