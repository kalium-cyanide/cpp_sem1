//
// Created by denis on 20.10.2025.
//

#include "board_widget.hpp"

template <class t>
void matrix_for_each(const matrix<t> &value, auto functor) {
  for (int x = 0; x < value.size(); x++) {
    for (int y = 0; y < value[x].size(); y++) {
      functor(x, y);
    }
  }
}

BoardWidget::BoardWidget(int x, int y, int width, int height,
                         CellState white_cell_state, CellState black_cell_state,
                         BoardState &board_state, const char *label,
                         map<string_t, Fl_SVG_Image *> &piece_images)
    : board_state_(board_state),
      white_cell_state_(white_cell_state),
      black_cell_state_(black_cell_state),
      Fl_Widget(x, y, width, height, label),
      piece_images_(piece_images) {
  construct_matrix();
  update();
}

void BoardWidget::construct_matrix() {
  cells_ = std::vector(
      board_state_.size.x,
      std::vector(board_state_.size.y, static_cast<CellWidget *>(nullptr)));

  matrix_for_each(cells_, [this](int x, int y) {
    auto &current_cell_state =
        (x + y) % 2 ? black_cell_state_ : white_cell_state_;

    cells_[x][y] = new CellWidget(
        x * current_cell_state.cell_size, y * current_cell_state.cell_size,
        current_cell_state.cell_size, current_cell_state.cell_size,
        current_cell_state.main_color, current_cell_state.hovered_color,
        std::to_string(x + y * 1000).c_str(), nullptr);
  });
}

void BoardWidget::draw() {
  auto draw_cell = [&](int x, int y) {
    auto &cell = cells_[x][y];

    cell->draw();
  };

  update();
  matrix_for_each(cells_, draw_cell);
}

void BoardWidget::update() {
  auto update_cell = [&](int x, int y) {
    auto cell = cells_[x][y];
    auto figure = board_state_.figures_m[x][y];

    auto image = figure == nullptr
                     ? nullptr
                     : piece_images_[std::to_string(figure->getColor()) +
                                     figure->getType()];

    cell->set_image(image);
  };

  matrix_for_each(cells_, update_cell);
}
int BoardWidget::handle(int event) {
  switch (event) {
    default:
      return Fl_Widget::handle(event);
  }
}
