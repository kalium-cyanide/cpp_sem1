
//
// Created by denis on 23.10.2025.
//

#include <iostream>
#include <map>
#include <memory>

#include "Board.hpp"
#include "FL/Fl.H"
#include "FL/Fl_Button.H"
#include "FL/Fl_Double_Window.H"
#include "FL/Fl_SVG_Image.H"
#include "FL/Fl_Widget.H"
#include "FL/fl_draw.H"
#include "GameCores/ClassicChessCore.hpp"
#include "board_widget.hpp"

std::map<string_t, Fl_SVG_Image*> m_piece_images;

void loadPieceImages() {
  const char* colors[] = {"w", "b"};
  const char* types[] = {"p", "n", "b", "r", "q", "k"};
  string_t backend_types[] = {"pawn", "knight", "bishop",
                              "rook", "queen",  "king"};

  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 6; ++j) {
      string_t key = (colors[i][0] == 'w' ? "0" : "1") + backend_types[j];
      string_t path =
          "assets/" + string_t(colors[i]) + string_t(types[j]) + ".svg";

      auto img = new Fl_SVG_Image(path.c_str());
      if (img->fail()) {
        std::cerr << "Error: Could not load image " << path << std::endl;
      } else {
        m_piece_images[key] = img;
      }
    }
  }
}

int main() {
  const int window_size = 600;
  auto* window = new Fl_Double_Window(window_size, window_size, "Chess");

  loadPieceImages();

  auto* game = new ClassicChessCore();

  auto state = game->getBoard()->getBoardState();

  auto white_color = fl_rgb_color(240, 217, 181);
  auto black_color = fl_rgb_color(181, 136, 99);
  auto yellow = fl_rgb_color(0, 255, 0);

  auto* board_w = new BoardWidget(
      0, 0, window_size, window_size,
      CellState(white_color, fl_color_average(white_color, yellow, 0.9),
                window_size / 8),
      CellState(black_color, fl_color_average(black_color, yellow, 0.9),
                window_size / 8),
      *state, "aa", m_piece_images);

  board_w->activate();

  window->show();
  return Fl::run();
}