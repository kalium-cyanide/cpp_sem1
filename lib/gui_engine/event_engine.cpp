//
// Created by denis on 20.11.2025.
//

#include "event_engine.hpp"
#include <sol/sol.hpp>

namespace {
    void find_widgets_recursive(Fl_Group *parent,
                                std::string_view selector,
                                std::vector<HtmlWidget *> &out_widgets) {
        for (int i = 0; i < parent->children(); ++i) {
            Fl_Widget *child = parent->child(i);

            if (auto *hw = dynamic_cast<HtmlWidget *>(child)) {
                bool match = false;

                if (selector[0] == '#') {
                    if (hw->id() == selector.substr(1)) match = true;
                } else if (selector[0] == '.') {
                    if (hw->has_class(selector.substr(1))) match = true;
                } else {
                    if (hw->tag() == selector) match = true;
                }

                if (match) {
                    out_widgets.push_back(hw);
                }

                find_widgets_recursive(hw, selector, out_widgets);
            } else if (auto *group = dynamic_cast<Fl_Group *>(child)) {
                find_widgets_recursive(group, selector, out_widgets);
            }
        }
    }
}// namespace

selected_elements event_engine::query(std::string_view selector) {
    std::vector<HtmlWidget *> found;

    if (fltk_root) {
        find_widgets_recursive(fltk_root, selector, found);
    }

    return selected_elements(std::move(found));
}

void event_engine::set_root(Fl_Group *root) {
    this->fltk_root = root;
}

event_engine::event_engine(sol::state &lua) : fltk_root(nullptr), lua(lua) {

    lua.new_usertype<HtmlWidget::EventState>("EventState",
                                             "mx", &HtmlWidget::EventState::mx,
                                             "my", &HtmlWidget::EventState::my);

    lua.new_enum("EventType",
                 "Push", HtmlWidget::EventType::Push,             // 0
                 "Drag", HtmlWidget::EventType::Drag,             // 1
                 "Click", HtmlWidget::EventType::Click,           // 2
                 "MouseEnter", HtmlWidget::EventType::MouseEnter, // 3
                 "MouseLeave", HtmlWidget::EventType::MouseLeave);// 4


    lua.new_usertype<selected_elements>("selected_elements",
                                        "apply", &selected_elements::apply,
                                        "get", &selected_elements::get);

    lua.new_usertype<HtmlWidget>("HtmlWidget",
                                 "set_style", &HtmlWidget::set_style,
                                 "get_style", &HtmlWidget::get_style,
                                 "w", &HtmlWidget::w,
                                 "h", &HtmlWidget::h,
                                 "x", &HtmlWidget::x,
                                 "y", &HtmlWidget::y,
                                 "parent", &HtmlWidget::parent,
                                 "id", &HtmlWidget::id,
                                 "has_class", &HtmlWidget::has_class,
                                 "tag", &HtmlWidget::tag,
                                 "classes", &HtmlWidget::classes,
                                 "callback", &HtmlWidget::set_callback);


    lua.set_function("query", [&](const std::string &selector) {
        return this->query(selector);
    });
}

selected_elements::selected_elements(std::vector<HtmlWidget *> &&selected) : selected(selected) {
}

void selected_elements::apply(const sol::function &callback) {
    auto wrapper = [callback](HtmlWidget *widget, HtmlWidget::EventType type, HtmlWidget::EventState state) {
        if (not callback.valid()) { return; }

        auto result = callback(widget, type, state);
        if (not result.valid()) {
            std::cerr << sol::error(result).what() << '\n';
        }
    };

    std::for_each(selected.begin(), selected.end(),
                  [&wrapper](auto i) {
                      HtmlWidget::EventCallback cb = wrapper;
                      i->set_callback(cb);
                  });
}
HtmlWidget *selected_elements::get(int N) {
    if (N >= selected.size()) { return nullptr; }
    return this->selected[N];
}
