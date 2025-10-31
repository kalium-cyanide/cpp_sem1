//
// Created by denis on 20.10.2025.
//

#pragma once

#include "FL/Fl_SVG_Image.H"
#include "FL/Fl_Widget.H"
#include "Figures/IFigure.hpp"

struct CellState {
  Fl_Color main_color;
  Fl_Color hovered_color;
  int cell_size;
};

class CellWidget : public Fl_Widget {
  Fl_SVG_Image* figure_;
  Fl_Color hovered_color_;
  Fl_Color main_color_;

 public:
  CellWidget(int x, int y, int width, int height, Fl_Color color,
             Fl_Color hovered_color, const char* label, Fl_SVG_Image* figure);

  int handle(int event) final;

  void resize(int x, int y, int w, int h) final;

  void set_image(Fl_SVG_Image* figure);

  void draw() final;
};
