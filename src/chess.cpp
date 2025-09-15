#include "Board.hpp"
#include "Boards/ClassicBoard.hpp"
#include "Boards/IBoard.hpp"
#include "Figure.hpp"
#include "Figures/Bishop.hpp"
#include "Figures/IFigure.hpp"
#include "Figures/Knight.hpp"
#include "Figures/Queen.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

#include "PossibleMovesGenerator.hpp"

enum colors
{
    WHITE = 0,
    BLACK = 1
};

wchar_t to_wchar(IFigure* figure)
{
    if (figure == nullptr) { return L'.'; }
    if (dynamic_cast<Pawn*>(figure) != nullptr) { return figure->getColor() == WHITE ? L'\u2659' : L'\u265F'; }
    if (dynamic_cast<King*>(figure) != nullptr) { return figure->getColor() == WHITE ? L'\u2654' : L'\u265A'; }
    if (dynamic_cast<Knight*>(figure) != nullptr) { return figure->getColor() == WHITE ? L'\u2658' : L'\u265E'; }
    if (dynamic_cast<Bishop*>(figure) != nullptr) { return figure->getColor() == WHITE ? L'\u2657' : L'\u265D'; }
    if (dynamic_cast<Queen*>(figure) != nullptr) { return figure->getColor() == WHITE ? L'\u2655' : L'\u265B'; }
    if (dynamic_cast<Rook*>(figure) != nullptr) { return figure->getColor() == WHITE ? L'\u2656' : L'\u265C'; }
    return L'?';  // на случай, если тип фигуры неизвестен
}
void drawClassicBoard(ClassicBoard& board_a)
{
    for (auto i = 1; i <= 8; i++)
    {
        for (auto j = 1; j <= 8; j++)
            std::wcout << to_wchar(board_a.getFigure(position_t(i, j)));  // board_a.getFigure((i,j))
        std::wcout << '\n';
    }
}

void setFigures(IBoard& board_a)
{
    for (int row = 1; row <= 8; row++)
    {
        for (int col = 1; col <= 8; col++)
        {
            board_a.setFigure(position_t(row, col), nullptr);
        }
    }

    for (int i = 1; i < 9; i++)
    {
        board_a.setFigure(position_t(2, i), new Pawn(WHITE));
        board_a.setFigure(position_t(7, i), new Pawn(BLACK));
    }

    for (auto color : {0, 1})
    {
        board_a.setFigure(position_t(1 + color * 7, 1), new Rook(color));
        board_a.setFigure(position_t(1 + color * 7, 2), new Knight(color));
        board_a.setFigure(position_t(1 + color * 7, 3), new Bishop(color));
        board_a.setFigure(position_t(1 + color * 7, 4), new Queen(color));
        board_a.setFigure(position_t(1 + color * 7, 5), new King(color));
        board_a.setFigure(position_t(1 + color * 7, 6), new Bishop(color));
        board_a.setFigure(position_t(1 + color * 7, 7), new Knight(color));
        board_a.setFigure(position_t(1 + color * 7, 8), new Rook(color));
    }
}

int main(int argc, char** argv)
{
    ClassicBoard board;
    setlocale(LC_ALL, "en_US.utf8");
    SetConsoleOutputCP(CP_UTF8);

    setFigures(board);
    drawClassicBoard(board);

    return 0;
}
