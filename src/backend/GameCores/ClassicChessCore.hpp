#pragma once

#include "../Validators/Validator.hpp"
#include "../utility.hpp"
#include "IGameCore.hpp"

class IBoard;
class IPlayer;
class IFigure;
struct Move;
struct ValidatorInput;
struct ValidatorOutput;

class ClassicChessCore : public IGameCore {
private:
    array_t<IPlayer *> players_m;
    IBoard *board;

    color_t currentPlayerIndex_m;
    Validator validator_m;
    bool isGameActive_m;
    possible_moves_table *possible_move_table_m;

    array_t<IFigure *> capturedFigures;
    array_t<std::tuple<IPlayer *, IFigure *, Move>> *move_history;

    void initializePlayers();
    void initializeBoard();
    void initializeValidator();

    void setupClassicFigures(IBoard &board_a);

    wchar_t to_wchar(IFigure *figure);

    void captureFigure(Move &move);
    void moveFigure(Move &move);
    void castling(Move &move);
    void upgradePawn();
    bool hasAnyValidMoves(color_t playerColor);

    ValidatorOutput moveValidate(IPlayer *player, Move &move);

    void gameStep();

    int gameResult = -1;

public:
    ClassicChessCore();
    ~ClassicChessCore();

    void startGame() override;
    void closeGame() override;
    int getGameResult() const;


    array_t<IPlayer *> getPlayers() override;
    IBoard *getBoard() override;

    void setPlayers(IPlayer *player1, IPlayer *player2);
    std::vector<position_t> getLegalMovesForCell(int col, int row);
    bool isKingInCheck(color_t playerColor);
};