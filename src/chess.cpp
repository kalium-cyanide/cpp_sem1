#include "Board.hpp"
#include "Figure.hpp"
#include <iostream>
#include <vector>

template <class t> using matrix = std::vector<std::vector<t>>;

auto board = std::vector(10, std::vector(10, char()));

void draw(matrix<char> board_a)
{
    for (auto line : board_a)
    {
        for (auto el : line)
            std::cout << el;
        std::cout << '\n';
    }
}

void setFigures() {}

int main(int argc, char** argv) { return 0; }