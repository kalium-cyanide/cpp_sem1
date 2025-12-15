#pragma once

#include "IFigure.hpp"

#include <vector>

class Pawn : public IFigure {
    color_t color_m;

public:
    Pawn(color_t color) : color_m(color) {}

    color_t getColor() override { return color_m; }
    void setColor(color_t color) override { color_m = color; }

    bool isPossibleMove(const Move &move) override {
        position_t offset = move.to - move.from;

        if (offset.x != 0) { return false; }
        if (offset.y == 1 or offset.y == 2) { return true; }

        return false;
    }

    string_t getType() override { return "pawn"; }
};
