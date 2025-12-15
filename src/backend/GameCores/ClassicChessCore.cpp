#include "ClassicChessCore.hpp"

#include <algorithm>
#include <iostream>
#include <iterator>

#include "../Board.hpp"
#include "../DataFabric.hpp"
#include "../Figure.hpp"
#include "../Players.hpp"
#include "../Validators/ClassicChessValidations.hpp"

void ClassicChessCore::startGame() {
    isGameActive_m = true;

    while (isGameActive_m) {


        gameStep();

        currentPlayerIndex_m = (currentPlayerIndex_m + 1) % players_m.size();
    }
}

ValidatorOutput ClassicChessCore::moveValidate(IPlayer *player,
                                               const Move &move) {
    std::cout << "move validation start\n";

    ValidatorInput input = {possible_move_table_m, board, player, move};
    ValidatorOutput output;
    output.result = true;

    validator_m.validate(input, output);
    std::cout << "move validation end\n";

    return output;
}

void ClassicChessCore::captureFigure(const Move &move) {
    IFigure *figureToMove = board->getFigure(move.from);
    IFigure *capturedFigure = board->getFigure(move.to);

    capturedFigures.push_back(capturedFigure);
    board->removeFigure(move.to);
}

void ClassicChessCore::moveFigure(const Move &move) {
    if (move.special == Move::SHORT_CASTLING or
        move.special == Move::LONG_CASTLING) {
        castling(move);
        return;
    }

    IPlayer *currentPlayer = players_m[currentPlayerIndex_m];

    IFigure *figureToMove = board->getFigure(move.from);
    IFigure *capturedFigure = board->getFigure(move.to);

    if (capturedFigure != nullptr) {
        captureFigure(move);
    }

    board->setFigure(move.to, figureToMove);
    board->removeFigure(move.from);

    move_history->push_back({currentPlayer, figureToMove, move});
}

bool ClassicChessCore::isKingInCheck(color_t playerColor) {
    position_t kingPos = {-1, -1};
    auto size = board->getBoardState()->size;

    for (int x = 1; x <= size.x; ++x) {
        for (int y = 1; y <= size.y; ++y) {
            IFigure *fig = board->getFigure({x, y});
            if (fig && fig->getType() == "king" && fig->getColor() == playerColor) {
                kingPos = {x, y};
                break;
            }
        }
    }

    return ClassicChessValidations::isUnderAttack(board, kingPos, playerColor);
}

bool ClassicChessCore::hasAnyValidMoves(color_t playerColor) {
    auto size = board->getBoardState()->size;

    IPlayer *tempPlayer = nullptr;

    for (auto p: players_m) {
        if (p->getColor() == playerColor) tempPlayer = p;
    }

    for (int x = 1; x <= size.x; ++x) {
        for (int y = 1; y <= size.y; ++y) {

            position_t from(x, y);
            IFigure *fig = board->getFigure(from);

            if (fig == nullptr || fig->getColor() != playerColor) continue;

            for (int tx = 1; tx <= size.x; ++tx) {
                for (int ty = 1; ty <= size.y; ++ty) {
                    position_t to(tx, ty);
                    Move move = {from, to, Move::EMPTY};

                    if (fig->isPossibleMove(move) || (fig->getType() == "pawn")) {

                        ValidatorInput input = {possible_move_table_m, board, tempPlayer, move, move_history};
                        ValidatorOutput output;
                        validator_m.validate(input, output);

                        if (output.result) return true;
                    }
                }
            }
        }
    }
    return false;
}

void ClassicChessCore::gameStep() {
    IPlayer *currentPlayer = players_m[currentPlayerIndex_m];
    board->getBoardState()->player_to_move = currentPlayer->getColor();


    bool inCheck = isKingInCheck(currentPlayer->getColor());
    bool hasMoves = hasAnyValidMoves(currentPlayer->getColor());

    if (!hasMoves) {
        isGameActive_m = false;
        if (inCheck) {
            int loserColor = currentPlayer->getColor();
            gameResult = (loserColor == 0) ? 1 : 0;

            std::cout << "Checkmate! Player " << (loserColor == 0 ? "White" : "Black") << " lost." << std::endl;
        } else {
            gameResult = 2;
            std::cout << "Stalemate! It is draw." << std::endl;
        }
        return;
    }

    if (inCheck) {
        std::cout << "CHECK!" << std::endl;
    }

    bool moveWasSuccessful = false;

    while (!moveWasSuccessful && isGameActive_m) {
        Move currentMove = currentPlayer->doMove();

        ValidatorInput input = {possible_move_table_m, board, currentPlayer, currentMove, move_history};
        ValidatorOutput output;
        validator_m.validate(input, output);

        if (output.result) {
            moveFigure(currentMove);
            moveWasSuccessful = true;
        } else {
            std::cout << "Invalid move:\n";
            for (const auto &error: output.errors) {
                std::cout << "- " << error << std::endl;
            }
        }
    }
}

void ClassicChessCore::castling(const Move &move) {
    auto currentPlayer = players_m[currentPlayerIndex_m];

    auto isLongCastling = move.special == Move::LONG_CASTLING;

    auto kingPos = position_t{5, (currentPlayer->getColor() == 0) ? (1) : (8)};
    auto rookPos = position_t{isLongCastling ? (1) : (8),
                              (currentPlayer->getColor() == 0) ? (1) : (8)};

    auto king = board->getFigure(kingPos);
    auto rook = board->getFigure(rookPos);

    board->removeFigure(kingPos);
    board->removeFigure(rookPos);

    board->setFigure({isLongCastling ? (3) : (7), kingPos.y}, king);
    board->setFigure({isLongCastling ? (4) : (6), rookPos.y}, rook);
}

void ClassicChessCore::upgradePawn() {}

void ClassicChessCore::initializePlayers() {
}

void ClassicChessCore::initializeBoard() {
    board = new ClassicBoard();
    setupClassicFigures(*board);
}

void ClassicChessCore::initializeValidator() {
    std::vector<IFigure *> figure_prototypes = {new Pawn(0), new Knight(0),
                                                new Rook(0), new Bishop(0),
                                                new Queen(0), new King(0)};
    possible_move_table_m =
            DataFabric::createPossibleMoveTable(board, figure_prototypes);

    validator_m.use(ClassicChessValidations::CastlingValidation);

    validator_m.use(ClassicChessValidations::BoardBorderValidation);
    validator_m.use(ClassicChessValidations::FigureExistenceValidation);
    validator_m.use(ClassicChessValidations::ColorValidation);

    validator_m.use(ClassicChessValidations::FirstPawnMoveValidation);
    validator_m.use(ClassicChessValidations::PawnAttackValidation);

    validator_m.use(ClassicChessValidations::PathClearValidation);
    validator_m.use(ClassicChessValidations::FigureMovePatternValidation);

    validator_m.use(ClassicChessValidations::KingSafetyValidation);

    for (auto fig: figure_prototypes) delete fig;
}

void ClassicChessCore::setupClassicFigures(IBoard &board_a) {
    for (int row = 1; row <= 8; row++) {
        for (int col = 1; col <= 8; col++) {
            board_a.setFigure(position_t(col, row), nullptr);
        }
    }

    for (int col = 1; col <= 8; col++) {
        board_a.setFigure(position_t(col, 2), new Pawn(0));
        board_a.setFigure(position_t(col, 7), new Pawn(1));
    }

    board_a.setFigure(position_t(1, 1), new Rook(0));
    board_a.setFigure(position_t(8, 1), new Rook(0));
    board_a.setFigure(position_t(2, 1), new Knight(0));
    board_a.setFigure(position_t(7, 1), new Knight(0));
    board_a.setFigure(position_t(3, 1), new Bishop(0));
    board_a.setFigure(position_t(6, 1), new Bishop(0));
    board_a.setFigure(position_t(4, 1), new Queen(0));
    board_a.setFigure(position_t(5, 1), new King(0));

    board_a.setFigure(position_t(1, 8), new Rook(1));
    board_a.setFigure(position_t(8, 8), new Rook(1));
    board_a.setFigure(position_t(2, 8), new Knight(1));
    board_a.setFigure(position_t(7, 8), new Knight(1));
    board_a.setFigure(position_t(3, 8), new Bishop(1));
    board_a.setFigure(position_t(6, 8), new Bishop(1));
    board_a.setFigure(position_t(4, 8), new Queen(1));
    board_a.setFigure(position_t(5, 8), new King(1));
}

wchar_t ClassicChessCore::to_wchar(IFigure *figure) {
    if (figure == nullptr) {
        return L'.';
    }
    if (dynamic_cast<King *>(figure) != nullptr) {
        return figure->getColor() == 0 ? L'\u2654' : L'\u265A';
    }
    if (dynamic_cast<Queen *>(figure) != nullptr) {
        return figure->getColor() == 0 ? L'\u2655' : L'\u265B';
    }
    if (dynamic_cast<Rook *>(figure) != nullptr) {
        return figure->getColor() == 0 ? L'\u2656' : L'\u265C';
    }
    if (dynamic_cast<Bishop *>(figure) != nullptr) {
        return figure->getColor() == 0 ? L'\u2657' : L'\u265D';
    }
    if (dynamic_cast<Knight *>(figure) != nullptr) {
        return figure->getColor() == 0 ? L'\u2658' : L'\u265E';
    }
    if (dynamic_cast<Pawn *>(figure) != nullptr) {
        return figure->getColor() == 0 ? L'\u2659' : L'\u265F';
    }
    return L'?';
}

ClassicChessCore::ClassicChessCore()
    : currentPlayerIndex_m(0), isGameActive_m(false) {
    move_history = new array_t<std::tuple<IPlayer *, IFigure *, Move>>();


    initializeBoard();
    initializeValidator();
}

ClassicChessCore::~ClassicChessCore() {
    for (auto player: players_m) {
        delete player;
    }

    delete board;

    delete possible_move_table_m;
    delete move_history;
}

void ClassicChessCore::closeGame() {
    isGameActive_m = false;
    std::cout << "Game has ended." << std::endl;
}

array_t<IPlayer *> ClassicChessCore::getPlayers() { return players_m; }

IBoard *ClassicChessCore::getBoard() { return board; }

void ClassicChessCore::setPlayers(IPlayer *player1, IPlayer *player2) {
    for (auto p: players_m) delete p;
    players_m.clear();
    players_m.push_back(player1);
    players_m.push_back(player2);
}
int ClassicChessCore::getGameResult() const { return gameResult; }
