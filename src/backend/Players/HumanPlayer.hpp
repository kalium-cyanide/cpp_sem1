#pragma once

#include "IPlayer.hpp"
#include <algorithm>
#include <iostream>
#include <string>

class HumanPlayer : public IPlayer {
    color_t color_m;
    string_t name_m;

    position_t parsePosition(const std::string &s) {
        if (s.length() != 2) { return {-1, -1}; }

        int col = tolower(s[0]) - 'a' + 1;
        int row = s[1] - '0';

        if (col < 1 or col > 8 or row < 1 or row > 8) { return {-1, -1}; }
        return position_t(col, row);
    }

public:
    HumanPlayer(color_t color, string_t name) : color_m(color), name_m(name) {}

    Move doMove() {
        std::string inputStr, fromStr, toStr;
        position_t from, to;

        while (true) {
            std::cout << name_m << " (" << (color_m == 0 ? "WHITE" : "BLACK") << "), enter your move (e2 e4): ";
            std::getline(std::cin, inputStr);

            if (inputStr == "0-0") { return Move(position_t(), position_t(), Move::SHORT_CASTLING); }
            if (inputStr == "0-0-0") { return Move(position_t(), position_t(), Move::LONG_CASTLING); }

            auto spaceIter = std::find(inputStr.begin(), inputStr.end(), ' ');

            fromStr = std::string(inputStr.begin(), spaceIter);
            toStr = std::string(spaceIter + 1, inputStr.end());

            from = parsePosition(fromStr);
            to = parsePosition(toStr);

            if (from.x != -1 && to.x != -1) {
                return {from, to};
            } else {
                std::cout << "invalid format:  use algebraic notation." << std::endl;
                std::cin.clear();
            }
        }
    }

    string_t getName() {
        return name_m;
    }

    color_t getColor() {
        return color_m;
    }
};