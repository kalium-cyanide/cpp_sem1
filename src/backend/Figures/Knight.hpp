#pragma once

#include "IFigure.hpp"

#include <vector>

class Knight : public IFigure {
    color_t color_m;

public:
    Knight(color_t color) : color_m(color) {}

    color_t getColor() override {
        return color_m;
    }
    void setColor(color_t color) override {
        color_m = color;
    }

    bool isPossibleMove(const Move &move) override {
        position_t offset = move.to - move.from;

        if (std::abs(offset.x) == 2 and std::abs(offset.y) == 1) { return true; }
        if (std::abs(offset.x) == 1 and std::abs(offset.y) == 2) { return true; }

        return false;
    }

    string_t getType() override {
        return "knight";
    }
};
