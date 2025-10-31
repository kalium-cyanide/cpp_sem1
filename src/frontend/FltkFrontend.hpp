#pragma once

#include "FL/Fl.H"
#include "FL/Fl_Double_Window.H"
#include "FL/Fl_SVG_Image.H"
#include "FL/fl_draw.H"

#include <condition_variable>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <thread>

#include "../backend/Board.hpp"
#include "../backend/Figure.hpp"
#include "../backend/Figures/IFigure.hpp"
#include "../backend/Move.hpp"
#include "../backend/Players.hpp"

class ChessBoardWidget : public Fl_Widget {
 public:
  inline ChessBoardWidget(int x, int y, int w, int h, IBoard* board_ref);
  inline ~ChessBoardWidget();

  inline void setHumanPlayers(const array_t<IPlayer*>& players);

 private:
  void draw() override;
  int handle(int event) override;

  void loadPieceImages();
  void drawBoard();
  void drawPieces();
  void drawDraggedPiece();
  position_t pixelToBoard(int x, int y) const;

  IBoard* m_board_ref;

  array_t<IPlayer*> m_human_players_ref;
  int m_square_size;

  std::map<string_t, std::unique_ptr<Fl_SVG_Image>> m_piece_images;

  bool m_is_dragging = false;
  IFigure* m_dragged_piece = nullptr;
  position_t m_drag_from_pos;
  int m_drag_current_x;
  int m_drag_current_y;
};

inline ChessBoardWidget::ChessBoardWidget(int x, int y, int w, int h,
                                          IBoard* board_ref)
    : Fl_Widget(x, y, w, h), m_board_ref(board_ref) {
  m_square_size = w / 8;
  loadPieceImages();
}

inline ChessBoardWidget::~ChessBoardWidget() {}

inline void ChessBoardWidget::setHumanPlayers(
    const array_t<IPlayer*>& players) {
  m_human_players_ref = players;
}

inline void ChessBoardWidget::loadPieceImages() {
  const char* colors[] = {"w", "b"};
  const char* types[] = {"p", "n", "b", "r", "q", "k"};
  string_t backend_types[] = {"pawn", "knight", "bishop",
                              "rook", "queen",  "king"};

  for (int i = 0; i < 2; ++i) {
    for (int j = 0; j < 6; ++j) {
      string_t key = (colors[i][0] == 'w' ? "0" : "1") + backend_types[j];
      string_t path =
          "assets/" + string_t(colors[i]) + string_t(types[j]) + ".svg";

      auto img = std::make_unique<Fl_SVG_Image>(path.c_str());
      if (img->fail()) {
        std::cerr << "Error: Could not load image " << path << std::endl;
      } else {
        m_piece_images[key] = std::move(img);
      }
    }
  }
}

inline void ChessBoardWidget::draw() {
  int min_dim = (w() < h()) ? w() : h();
  size(min_dim, min_dim);
  m_square_size = w() / 8;

  drawBoard();
  drawPieces();
  if (m_is_dragging) {
    drawDraggedPiece();
  }
}

inline void ChessBoardWidget::drawBoard() {
  for (int r = 0; r < 8; ++r) {
    for (int c = 0; c < 8; ++c) {
      fl_color((r + c) % 2 == 0 ? fl_rgb_color(240, 217, 181)
                                : fl_rgb_color(181, 136, 99));
      fl_rectf(x() + c * m_square_size, y() + r * m_square_size, m_square_size,
               m_square_size);
    }
  }
}

inline void ChessBoardWidget::drawPieces() {
  if (!m_board_ref) return;
  for (int r = 1; r <= 8; ++r) {
    for (int c = 1; c <= 8; ++c) {
      IFigure* fig = m_board_ref->getFigure({c, r});
      if (fig) {
        if (m_is_dragging && m_drag_from_pos.x == c && m_drag_from_pos.y == r) {
          continue;
        }

        string_t key = std::to_string(fig->getColor()) + fig->getType();
        if (m_piece_images.count(key)) {
          auto& img = m_piece_images.at(key);
          img->resize(m_square_size, m_square_size);
          img->draw(x() + (c - 1) * m_square_size,
                    y() + (8 - r) * m_square_size);
        }
      }
    }
  }
}

inline void ChessBoardWidget::drawDraggedPiece() {
  if (!m_dragged_piece) return;
  string_t key =
      std::to_string(m_dragged_piece->getColor()) + m_dragged_piece->getType();
  if (m_piece_images.count(key)) {
    auto& img = m_piece_images.at(key);
    img->resize(m_square_size, m_square_size);
    img->draw(m_drag_current_x - m_square_size / 2,
              m_drag_current_y - m_square_size / 2);
  }
}

inline position_t ChessBoardWidget::pixelToBoard(int mx, int my) const {
  int col = (mx - x()) / m_square_size + 1;
  int row = 8 - (my - y()) / m_square_size;
  return {col, row};
}



inline int ChessBoardWidget::handle(int event) {
  if (m_human_players_ref.empty() || !m_board_ref) return 0;

  color_t current_turn_color = m_board_ref->getBoardState()->player_to_move;

  GuiHumanPlayer* active_player = nullptr;
  for (auto* player : m_human_players_ref) {
    if (player->getColor() == current_turn_color) {
      active_player = dynamic_cast<GuiHumanPlayer*>(player);
      break;
    }
  }

  if (!active_player) {
    return 0;
  }

  position_t pos = pixelToBoard(Fl::event_x(), Fl::event_y());

  switch (event) {
    case FL_PUSH: {
      if (pos.x >= 1 && pos.x <= 8 && pos.y >= 1 && pos.y <= 8) {
        IFigure* fig = m_board_ref->getFigure(pos);

        if (fig && fig->getColor() == active_player->getColor()) {
          m_is_dragging = true;
          m_dragged_piece = fig;
          m_drag_from_pos = pos;
          m_drag_current_x = Fl::event_x();
          m_drag_current_y = Fl::event_y();
          redraw();
          return 1;
        }
      }
      break;
    }
    case FL_DRAG: {
      if (m_is_dragging) {
        m_drag_current_x = Fl::event_x();
        m_drag_current_y = Fl::event_y();
        redraw();
        return 1;
      }
      break;
    }
    case FL_RELEASE: {
      if (m_is_dragging) {
        m_is_dragging = false;
        if (pos.x >= 1 && pos.x <= 8 && pos.y >= 1 && pos.y <= 8) {
          Move user_move = {m_drag_from_pos, pos};

          auto fromF = m_board_ref->getFigure(user_move.from);
          auto toF = m_board_ref->getFigure(user_move.to);

          if ((fromF->getType() == "rook" and toF->getType() == "king") or
              (fromF->getType() == "king" and toF->getType() == "rook")) {
            if (std::abs(user_move.from.x - user_move.to.x) == 3) {
              active_player->provideMove(
                  Move(position_t(), position_t(), Move::SHORT_CASTLING));
            } else {
              active_player->provideMove(
                  Move(position_t(), position_t(), Move::LONG_CASTLING));
            }
          } else {
            active_player->provideMove(user_move);
          }
        }
        redraw();
        return 1;
      }
      break;
    }
  }
  return Fl_Widget::handle(event);
}
