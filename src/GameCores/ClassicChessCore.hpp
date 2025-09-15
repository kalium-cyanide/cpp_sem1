#include "IGameCore.hpp"

class ClassicChessCore
{
    ClassicBoard board;

  public:
    void startGame()
    {}

    void closeGame()
    {}

    array_t<IPlayer*> getPlayers()
    {}

    array_t<IBoard*> getBoard()
    {}
};