#pragma once

#include "IPlayer.hpp"

class HumanPlayer : public IPlayer
{
    color_t color_m;

  public:
    HumanPlayer(color_t color) : color_m(color)
    {}

    Move doMove()
    {}

    string_t getName()
    {}

    color_t getColor()
    {}
};