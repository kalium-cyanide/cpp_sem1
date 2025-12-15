#pragma once

#include "BoardState.hpp"
#include "IBoard.hpp"
#include <iostream>
#include <vector>

class ClassicBoard : public IBoard {
    BoardState *state;

public:
    ClassicBoard() {
        state = new BoardState();
        state->size = position_t(8, 8);

        state->figures_m.resize(state->size.x);
        for (auto &i: state->figures_m) {
            i.resize(state->size.y);
        }
    }

    IFigure *getFigure(position_t position) override {
        return state->figures_m[position.x - 1][position.y - 1];
    }

    bool isPossibleSet(position_t position, IFigure *figure) override {
        if (not(0 < position.x and position.x <= state->size.x)) { return false; }
        if (not(0 < position.y and position.y <= state->size.y)) { return false; }
        return true;
    }

    void setFigure(position_t position, IFigure *figure) override {
        state->figures_m[position.x - 1][position.y - 1] = figure;
    }

    void removeFigure(position_t position) override {
        state->figures_m[position.x - 1][position.y - 1] = nullptr;
    }

    BoardState *getBoardState() override {
        return state;
    }

    void setBoardState() override {
        return;
    }
};
