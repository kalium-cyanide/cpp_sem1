#pragma once

#include "html_widget.hpp"
#include "layout_types.hpp"
#include <memory>

class Fl_Group;

namespace fltk_renderer {

    HtmlWidget *create_widget_tree(const LayoutBox *root_box);

    void update_widget_tree(HtmlWidget *root_widget, const LayoutBox *new_layout_root);
}// namespace fltk_renderer