#pragma once

#include <FL/Fl_Widget.H>

#include <map>
#include <memory>

#include "Boards/BoardState.hpp"
#include "cell_widget.hpp"

template <class t>
using matrix = std::vector<std::vector<t>>;

template <class u, class v>
using map = std::map<u, v>;

class BoardWidget : public Fl_Widget {
  matrix<CellWidget *> cells_;
  int cell_size;

  BoardState &board_state_;

  CellState black_cell_state_;
  CellState white_cell_state_;

  map<string_t, Fl_SVG_Image *> &piece_images_;

  void construct_matrix();

 public:
  BoardWidget(int x, int y, int width, int height, CellState white_cell_state,
              CellState black_cell_state, BoardState &board_state,
              const char *label, map<string_t, Fl_SVG_Image *> &piece_images);
  void draw() final;

  void update();

  int handle(int event) final;
};
