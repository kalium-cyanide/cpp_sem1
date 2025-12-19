#include "browser_window.hpp"
#include "css_parser.hpp"
#include "event_engine.hpp"
#include "fltk_renderer.hpp"
#include "layout_engine.hpp"
#include "preprocessor.hpp"
#include "style_engine.hpp"
#include "xml_parser.hpp"


#include "Figures/IFigure.hpp"
#include "GameCores/ClassicChessCore.hpp"
#include "Players/GUIPlayer.hpp"

#include <fstream>
#include <iostream>
#include <mutex>
#include <thread>

ClassicChessCore *game_core = nullptr;

std::string get_figure(int x, int y) {
    if (!game_core) return "";

    auto *board = game_core->getBoard();
    if (!board) return "";


    auto *fig = board->getFigure({x, y});

    if (fig == nullptr) return "";

    std::string type = fig->getType();
    int color = fig->getColor();

    std::string result = (color == 0 ? "w" : "b");
    if (type == "pawn") result += "p";
    else if (type == "rook")
        result += "r";
    else if (type == "knight")
        result += "n";
    else if (type == "bishop")
        result += "b";
    else if (type == "queen")
        result += "q";
    else if (type == "king")
        result += "k";

    return result;
}

void make_move(int from_col, int from_row, int to_col, int to_row) {
    if (!game_core) return;

    auto players = game_core->getPlayers();
    Move move;

    if (from_col == -1) {
        move.from = {0, 0};
        move.to = {0, 0};
        move.special = Move::SHORT_CASTLING;
    } else if (from_col == -2) {
        move.from = {0, 0};
        move.to = {0, 0};
        move.special = Move::LONG_CASTLING;
    } else {

        move.from = {from_col, from_row};
        move.to = {to_col, to_row};
        move.special = Move::EMPTY;
    }

    auto current = players[game_core->getBoard()->getBoardState()->player_to_move];
    if (auto gui_player = dynamic_cast<GuiHumanPlayer *>(current)) {
        gui_player->provideMove(move);
    }


    std::this_thread::sleep_for(std::chrono::milliseconds(20ll));
}

void game_thread_func() {
    if (game_core) {
        game_core->startGame();
    }
}

int get_game_status() {
    if (!game_core) return -1;
    return game_core->getGameResult();
}

// Новая функция для проверки шаха
bool is_king_in_check(int color) {
    if (!game_core) return false;
    return game_core->isKingInCheck(color);
}

void restart_game() {

    if (game_core) {
        game_core->closeGame();
    }

    game_core = new ClassicChessCore();
    auto *p1 = new GuiHumanPlayer(0, "White");
    auto *p2 = new GuiHumanPlayer(1, "Black");
    game_core->setPlayers(p1, p2);

    std::thread logic_thread(game_thread_func);
    logic_thread.detach();
}


void exit_app() {
    exit(0);
}

sol::table get_legal_moves_wrapper(sol::this_state s, int col, int row) {
    sol::state_view lua(s);
    sol::table result = lua.create_table();
    if (!game_core) return result;

    auto moves = game_core->getLegalMovesForCell(col, row);

    int index = 1;
    for (const auto &pos: moves) {
        sol::table pos_tbl = lua.create_table();
        pos_tbl["x"] = pos.x;
        pos_tbl["y"] = pos.y;
        result[index++] = pos_tbl;
    }
    return result;
}

void gui_init() {
    float viewport_width = 1000.0f;
    float viewport_height = 600.0f;


    game_core = new ClassicChessCore();
    auto *p1 = new GuiHumanPlayer(0, "White");
    auto *p2 = new GuiHumanPlayer(1, "Black");
    game_core->setPlayers(p1, p2);


    std::thread logic_thread(game_thread_func);
    logic_thread.detach();
    auto html_file = std::ifstream("../assets/ui.luax");
    auto css_file = std::ifstream("../assets/styles.css");


    if (!html_file || !css_file) {
        std::cerr << "Error: Assets not found!"
                  << std::endl;
        return;
    }

    std::string html_string((std::istreambuf_iterator<char>(html_file)), std::istreambuf_iterator<char>());

    preprocessor::preprocess(html_string);

    auto dom = xml_parser::parse(html_string);
    auto stylesheet = CssParser::parse(css_file);
    sol::state lua;
    lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::math, sol::lib::table);

    lua.set_function("get_figure", &get_figure);
    lua.set_function("make_move", &make_move);
    lua.set_function("cpp_get_status", &get_game_status);
    lua.set_function("cpp_restart_game", &restart_game);
    lua.set_function("cpp_exit_app", &exit_app);
    lua.set_function("get_legal_moves", &get_legal_moves_wrapper);

    // Регистрация новой функции
    lua.set_function("cpp_is_king_in_check", &is_king_in_check);

    event_engine events(lua);
    auto styled_root = style_engine::apply_styles(dom.get(), stylesheet);
    auto layout = layout_engine::calculate_layout(styled_root.get(), viewport_width, viewport_height);
    auto *window = new BrowserWindow(int(viewport_width), int(viewport_height), "Chess Game");
    window->set_styled_root(std::move(styled_root));

    window->on_update = [&lua]() {
        sol::function update = lua["update_view"];
        if (update.valid()) {
            update();
        }
    };
    events.set_root(window->get_root());

    try {
        lua.script_file("../assets/script.lua");
    } catch (const sol::error &e) {
        std::cerr << "Lua Error: " << e.what() << std::endl;
    }

    window->show();
    BrowserWindow::run();
}