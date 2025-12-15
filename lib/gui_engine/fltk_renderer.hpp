#pragma once

#include "layout_types.hpp"
#include <memory>

class Fl_Group;

namespace fltk_renderer {

    Fl_Group *create_widget_tree(const LayoutBox *root_box);

    void update_widget_tree(Fl_Group *root_widget, const LayoutBox *new_layout_root);
}// namespace fltk_renderer