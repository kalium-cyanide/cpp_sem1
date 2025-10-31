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
  array_t<IPlayer*> players_m;
  IBoard* board;

  color_t currentPlayerIndex_m;
  Validator validator_m;
  bool isGameActive_m;
  possible_moves_table* possible_move_table_m;

  array_t<IFigure*> capturedFigures;
  array_t<std::pair<IPlayer*, std::pair<IFigure*, Move>>>* move_history;

  void initializePlayers();
  void initializeBoard();
  void initializeValidator();

  void setupClassicFigures(IBoard& board_a);

  wchar_t to_wchar(IFigure* figure);

  void captureFigure(const Move& move);
  void moveFigure(const Move& move);
  void castling(const Move& move);
  void upgradePawn();
  ValidatorOutput moveValidate(IPlayer* player, const Move& move);

  void gameStep();

 public:
  ClassicChessCore();
  ~ClassicChessCore();

  void startGame() override;
  void closeGame() override;

  array_t<IPlayer*> getPlayers() override;
  IBoard* getBoard() override;

  void setPlayers(IPlayer* player1, IPlayer* player2);
};