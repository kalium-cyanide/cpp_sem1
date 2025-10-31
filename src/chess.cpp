// #include "GameCores/ClassicChessCore.hpp"

// int main()
// {
//     ClassicChessCore game;
//     game.startGame();
//     return 0;
// }
// chess.cpp
// chess.cpp

#include "backend/Boards/ClassicBoard.hpp"
#include "backend/DataFabric.hpp"
#include "backend/Figures/Bishop.hpp"
#include "backend/Figures/IFigure.hpp"
#include "backend/GameCores/ClassicChessCore.hpp"
#include "backend/Move.hpp"
#include "backend/Players/GUIPlayer.hpp"
#include "frontend/FltkFrontend.hpp"

ChessBoardWidget *g_board_widget = nullptr;
IGameCore *g_game_core = nullptr;

void game_thread_func()
{
    if (g_game_core)
    {
        g_game_core->startGame();
    }
}

int main()
{
    auto *board = new ClassicBoard();
    auto arr = array_t<IFigure *>{new Bishop(0)};
    auto a = DataFabric::createPossibleMoveTable(board, arr);

    for (auto i : *a)
    {
        std::cout << i.first << std::endl;
        for (auto j : i.second)
        {
            std::cout << '\t' << j << '\n';
        }
    }

    Fl::lock();

    const int window_size = 720;
    auto *window = new Fl_Double_Window(window_size, window_size, "Chess");

    auto *game = new ClassicChessCore();
    g_game_core = game;

    auto *player_white = new GuiHumanPlayer(0, "White Player");
    auto *player_black = new GuiHumanPlayer(1, "Black Player");

    game->setPlayers(player_white, player_black);

    g_board_widget = new ChessBoardWidget(0, 0, window_size, window_size, game->getBoard());

    array_t<IPlayer *> human_players = {player_white, player_black};
    g_board_widget->setHumanPlayers(human_players);

    window->end();
    window->resizable(g_board_widget);
    window->show();

    std::thread game_thread(game_thread_func);
    game_thread.detach();

    return Fl::run();
}