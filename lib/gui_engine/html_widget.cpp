#include "html_widget.hpp"
#include "layout_types.hpp"
#include <FL/Fl.H>
#include <FL/Fl_SVG_Image.H>
#include <FL/fl_draw.H>
#include <iostream>
#include <stdio.h>
#include <string>


Fl_Color rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    
    return fl_rgb_color(r, g, b);
}
Fl_Color HtmlWidget::parse_color(const std::string &css_color) {
    unsigned int r, g, b, a;
    if (css_color.rfind("rgba(", 0) == 0) {
        if (sscanf(css_color.c_str(), "rgba(%u,%u,%u,%u)", &r, &g, &b, &a) == 4) {
            return rgba(r, g, b, a);
        }
    } else if (css_color[0] == '#' and css_color.length() == 7) {
        if (sscanf(css_color.c_str() + 1, "%02x%02x%02x", &r, &g, &b) == 3) {
            return rgba(r, g, b, 255);
        }
    }


    return 0x00000000;
}

std::string HtmlWidget::get_style(const std::string &key) {
    if (styled_node_ref_) {
        return styled_node_ref_->styles[key];
    }
    return "";
}

static int parse_px(const std::string &val) {
    try {
        if (val.empty()) return 0;
        return std::stoi(val);
    } catch (...) {
        return 0;
    }
}

void HtmlWidget::set_style(const std::string &key, std::string value) {
    if (styled_node_ref_) {
        styled_node_ref_->styles[key] = value;
    }

    if (key == "display") {
        if (value == "none") this->hide();
        else
            this->show();
    } else if (key == "left" || key == "top" || key == "width" || key == "height") {
        int nx = (key == "left") ? parse_px(value) : x();
        int ny = (key == "top") ? parse_px(value) : y();
        int nw = (key == "width") ? parse_px(value) : w();
        int nh = (key == "height") ? parse_px(value) : h();

        this->resize(nx, ny, nw, nh);
        if (parent()) parent()->redraw();
    }
}

void HtmlWidget::draw_background() {
    std::string bg_color_str = get_style("background-color");
    if (bg_color_str.empty()) return;

    Fl_Color color = parse_color(bg_color_str);
    fl_color(color);
    fl_rectf(x(), y(), w(), h());
}

void HtmlWidget::draw_content_text() {
    if (!styled_node_ref_ || styled_node_ref_->ref->value.empty()) return;

    Fl_Color text_col = parse_color(get_style("color"));
    fl_color(text_col);

    fl_font(FL_HELVETICA, 14);
    fl_draw(styled_node_ref_->ref->value.c_str(),
            x() + 5, y() + 14, w() - 10, h() - 10,
            FL_ALIGN_LEFT | FL_ALIGN_WRAP);
}

HtmlWidget::HtmlWidget(int x, int y, int w, int h, StyledNode *node)
    : Fl_Group(x, y, w, h), styled_node_ref_(node), owned_image_(nullptr) {
}

HtmlWidget::~HtmlWidget() {
    if (owned_image_) delete owned_image_;
}

void HtmlWidget::image(Fl_Image *img_to_own) {
    delete owned_image_;
    owned_image_ = img_to_own;
}

void HtmlWidget::draw_image() {
    if (!owned_image_) return;
    if (auto svg = dynamic_cast<Fl_SVG_Image *>(owned_image_)) {
        svg->resize(w(), h());
    }
    owned_image_->draw(x(), y(), w(), h());
}

void HtmlWidget::draw() {
    draw_background();
    draw_image();
    if (!owned_image_) {
        draw_content_text();
    }
    Fl_Group::draw_children();
}

void HtmlWidget::add_event_listener(const std::string &type, EventListener listener) {
    listeners_[type].push_back(listener);
}

bool HtmlWidget::dispatch_event(DOMEvent &event) {
    std::vector<HtmlWidget *> propagation_path;
    HtmlWidget *curr = this;


    while (curr) {
        propagation_path.push_back(curr);
        if (curr->parent()) {
            curr = dynamic_cast<HtmlWidget *>(curr->parent());
        } else {
            curr = nullptr;
        }
    }


    for (auto *widget: propagation_path) {
        if (event.propagation_stopped) break;

        event.current_target = widget;

        if (widget->listeners_.count(event.type)) {
            for (const auto &callback: widget->listeners_[event.type]) {
                if (event.propagation_stopped) break;
                callback(event);
            }
        }
    }

    return !event.default_prevented;
}

int HtmlWidget::handle(int event) {


    if (event == FL_PUSH || event == FL_RELEASE) {
        if (Fl_Group::handle(event)) return 1;
    }


    DOMEvent dom_event;
    dom_event.client_x = Fl::event_x();
    dom_event.client_y = Fl::event_y();
    dom_event.target = this;

    switch (event) {
        case FL_PUSH:

            if (Fl::event_inside(this) and Fl::event_button() == FL_LEFT_MOUSE) {
                is_pressed_ = true;
                dom_event.type = "mousedown";
                dispatch_event(dom_event);
                return 1;
            }
            break;

        case FL_DRAG:

            dom_event.type = "mousemove";
            dispatch_event(dom_event);
            return 1;

        case FL_RELEASE:
            if (is_pressed_) {
                is_pressed_ = false;


                dom_event.type = "mouseup";
                dispatch_event(dom_event);


                if (Fl::event_inside(this)) {
                    dom_event.type = "click";
                    dom_event.propagation_stopped = false;
                    dispatch_event(dom_event);
                }
                return 1;
            }
            break;

        case FL_ENTER:
            dom_event.type = "mouseenter";
            dispatch_event(dom_event);
            return 1;

        case FL_LEAVE:
            dom_event.type = "mouseleave";
            dispatch_event(dom_event);
            return 1;
    }


    return Fl_Group::handle(event);
}

std::string HtmlWidget::id() {
    return styled_node_ref_->ref->attributes["id"];
}
std::string HtmlWidget::tag() {
    return styled_node_ref_->ref->name;
}
bool HtmlWidget::has_class(std::string_view class_name) {
    auto classes = this->classes();
    return classes.find(class_name) != std::string::npos;
}
std::string HtmlWidget::classes() {
    return styled_node_ref_->ref->attributes["class"];
}
void HtmlWidget::set_logical_parent(HtmlWidget *parent) {
    logical_parent_ = parent;
}
Fl_Widget *HtmlWidget::parent() const { return Fl_Group::parent(); }
void HtmlWidget::set_styled_node(StyledNode *node) {
    styled_node_ref_ = node;
}


std::function<void()> HtmlWidget::trigger_layout_update = nullptr;

void HtmlWidget::append_child(HtmlWidget *child) {
    if (!child || child == this) return;


    if (auto old_fltk_parent = child->parent()) {
        auto *old_group = dynamic_cast<Fl_Group *>(old_fltk_parent);
        if (old_group) {
            old_group->remove(child);
        }
    }
    this->add(child);


    StyledNode *child_s_node = child->get_node();
    StyledNode *new_parent_s_node = this->get_node();

    if (child_s_node and new_parent_s_node) {
        StyledNode *old_parent_s_node = child_s_node->parent;
        if (old_parent_s_node) {

            auto moved_s_node = detach_styled_node(old_parent_s_node, child_s_node);
            if (moved_s_node) {

                moved_s_node->parent = new_parent_s_node;
                new_parent_s_node->children.push_back(std::move(moved_s_node));
            }
        }
    }


    if (child_s_node and child_s_node->ref and new_parent_s_node and new_parent_s_node->ref) {
        DomNode *child_d_node = child_s_node->ref;
        DomNode *new_parent_d_node = new_parent_s_node->ref;
        DomNode *old_parent_d_node = child_d_node->parent;

        if (old_parent_d_node) {
            auto moved_d_node = detach_dom_node(old_parent_d_node, child_d_node);
            if (moved_d_node) {
                moved_d_node->parent = new_parent_d_node;
                new_parent_d_node->children.push_back(std::move(moved_d_node));
            }
        }
    }


    if (trigger_layout_update) {
        trigger_layout_update();
    }
}

Fl_Color apply_opacity(Fl_Color color, float opacity) {
    if (opacity >= 1.0f) return color;
    if (opacity <= 0.0f) return 0;

    unsigned char r, g, b;
    Fl::get_color(color, r, g, b);


    return color;
}