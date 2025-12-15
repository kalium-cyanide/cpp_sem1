#pragma once

#include "BoardState.hpp"

class IFigure;

struct IBoard {
    virtual IFigure *getFigure(position_t position) = 0;
    virtual void setFigure(position_t position, IFigure *figure) = 0;
    virtual void removeFigure(position_t position) = 0;

    virtual bool isPossibleSet(position_t position, IFigure *figure) = 0;

    virtual BoardState *getBoardState() = 0;
    virtual void setBoardState() = 0;
};
