#include "cell_widget.hpp"

#include "FL/fl_draw.H"

int CellWidget::handle(int event) {
  switch (event) {
    case Fl_Event::FL_ENTER:
      color(hovered_color_);
      redraw();

      return true;

    case Fl_Event::FL_LEAVE:
      color(main_color_);
      redraw();

      return true;

    default:
      return Fl_Widget::handle(event);
  }
}

CellWidget::CellWidget(int x, int y, int width, int height, Fl_Color main_color,
                       Fl_Color hovered_color, const char *label,
                       Fl_SVG_Image *figure)
    : Fl_Widget(x, y, width, height, label),
      main_color_(main_color),
      hovered_color_(hovered_color),
      figure_(figure) {
  Fl_Widget::color(main_color_);
}

void CellWidget::draw() {
  fl_color(color());
  fl_rectf(Fl_Rect(x(), y(), w(), h()));

  if (figure_ == nullptr) {
    return;
  }

  figure_->resize(w(), h());
  figure_->draw(x(), y());
}

void CellWidget::resize(int x, int y, int w, int h) {
  Fl_Widget::resize(x, y, w, h);
}

void CellWidget::set_image(Fl_SVG_Image *figure) { figure_ = figure; }
