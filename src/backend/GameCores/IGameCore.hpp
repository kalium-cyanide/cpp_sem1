#pragma once

#include "../Board.hpp"
#include "../Players.hpp"

struct IGameCore {
    virtual void startGame() = 0;
    virtual void closeGame() = 0;

    virtual array_t<IPlayer *> getPlayers() = 0;
    virtual IBoard *getBoard() = 0;
};