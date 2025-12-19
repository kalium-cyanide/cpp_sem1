//

//

#include "event_engine.hpp"
#include <sol/sol.hpp>

namespace {
    bool is_match(HtmlWidget *hw, std::string_view selector) {
        if (selector.empty()) return false;

        if (selector[0] == '#') {
            if (hw->id() == selector.substr(1)) return true;
        } else if (selector[0] == '.') {
            if (hw->has_class(selector.substr(1))) return true;
        } else {
            if (hw->tag() == selector) return true;
        }
        return false;
    }

    void find_widgets_recursive(Fl_Group *parent,
                                std::string_view selector,
                                std::vector<HtmlWidget *> &out_widgets) {
        for (int i = 0; i < parent->children(); ++i) {
            Fl_Widget *child = parent->child(i);


            if (auto *hw = dynamic_cast<HtmlWidget *>(child)) {
                if (is_match(hw, selector)) {
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

        if (auto *root_hw = dynamic_cast<HtmlWidget *>(fltk_root)) {
            if (is_match(root_hw, selector)) {
                found.push_back(root_hw);
            }
        }


        find_widgets_recursive(fltk_root, selector, found);
    }

    return selected_elements(std::move(found));
}
void event_engine::set_root(Fl_Group *root) {
    this->fltk_root = root;
}
selected_elements::selected_elements(std::vector<HtmlWidget *> &&selected) : selected(selected) {
}

event_engine::event_engine(sol::state &lua) : fltk_root(nullptr), lua(lua) {


    lua.new_usertype<DOMEvent>("Event",
                               "stopPropagation", &DOMEvent::stop_propagation,
                               "preventDefault", &DOMEvent::default_prevented,
                               "target", &DOMEvent::target,
                               "currentTarget", &DOMEvent::current_target,
                               "type", &DOMEvent::type,
                               "client_x", &DOMEvent::client_x,
                               "client_y", &DOMEvent::client_y);


    lua.new_usertype<selected_elements>("selected_elements",
                                        "get", &selected_elements::get,
                                        "on", &selected_elements::add_listener);


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
                                 "appendChild", &HtmlWidget::append_child,
                                 "addEventListener", [](HtmlWidget *self, std::string type, sol::function cb) {
                                     self->add_event_listener(type, [cb](DOMEvent &e) {
                                         if (cb.valid()) {

                                             cb(std::ref(e));
                                         }
                                     });
                                 });

    lua.set_function("query", [&](const std::string &selector) {
        return this->query(selector);
    });
}

void selected_elements::add_listener(const std::string &type, const sol::function &callback) {

    auto wrapper = [callback](DOMEvent &e) {
        if (callback.valid()) {
            auto result = callback(std::ref(e));
            if (!result.valid()) {
                sol::error err = result;
                std::cerr << "Lua Error: " << err.what() << '\n';
            }
        }
    };

    for (auto *widget: selected) {
        widget->add_event_listener(type, wrapper);
    }
}

HtmlWidget *selected_elements::get(int N) {
    if (N >= selected.size()) { return nullptr; }
    return this->selected[N];
}