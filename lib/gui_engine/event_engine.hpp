//
// Created by denis on 20.11.2025.
//

#pragma once
#include "dom_types.hpp"
#include "html_widget.hpp"
#include <sol/sol.hpp>
#include <vector>

class selected_elements {
    std::vector<HtmlWidget *> selected;

public:
    selected_elements(std::vector<HtmlWidget *> &&selected);

    void apply(const sol::function &callback);
    HtmlWidget *get(int N);
};

class event_engine {
    Fl_Group *fltk_root;

    sol::state &lua;

public:
    event_engine(sol::state &lua);

    void set_root(Fl_Group *root);

    selected_elements query(std::string_view selector);
};
