#pragma once

#include "BoardState.hpp"

class IFigure;

struct IBoard
{
    virtual IFigure* getFigure() = 0;
    virtual void setFigure() = 0;
    virtual void removeFigure() = 0;

    virtual BoardState getBoardState() = 0;
    virtual BoardState setBoardState() = 0;
};
