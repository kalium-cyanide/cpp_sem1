#pragma once
#include "dom_types.hpp"
#include "html_widget.hpp"
#include <sol/sol.hpp>
#include <vector>

class selected_elements {
    std::vector<HtmlWidget *> selected;

public:
    selected_elements(std::vector<HtmlWidget *> &&selected);

    void add_listener(const std::string &event_type, const sol::function &callback);

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