#include "html_widget.hpp"
#include "browser_window.hpp"
#include "layout_types.hpp"
#include <FL/Fl.H>
#include <FL/Fl_SVG_Image.H>
#include <FL/fl_draw.H>
#include <iostream>
#include <stdio.h>

Fl_Color rgba(uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    return uint32_t(a) + uint32_t(b << 8) + uint32_t(g << 16) + uint32_t(r << 24);
}

Fl_Color HtmlWidget::parse_color(const std::string &css_color) {
    unsigned int r, g, b, a;
    if (css_color.rfind("rgba(", 0) == 0) {
        if (sscanf(css_color.c_str(), "rgba(%u,%u,%u,%u)", &r, &g, &b, &a) == 4) {
            return rgba(r, g, b, a);
        }
    } else if (css_color[0] == '#' && css_color.length() == 7) {
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

void HtmlWidget::set_style(const std::string &key, std::string value) {
    if (key == "display") {
        if (value == "none") {
            this->hide();
        } else {
            this->show();
        }
    }

    if (styled_node_ref_) {
        styled_node_ref_->styles[key] = value;
    }
}

void HtmlWidget::draw_background() {
    std::string bg_color_str = get_style("background-color");
    if (bg_color_str.empty()) {
        return;
    }

    Fl_Color color = parse_color(bg_color_str);

    fl_color(color);
    fl_rectf(x(), y(), w(), h());
}

void HtmlWidget::draw_content_text() {
    if (!styled_node_ref_ || styled_node_ref_->ref->value.empty()) {
        return;
    }


    Fl_Color text_col = parse_color(get_style("color"));
    fl_color(text_col);

    fl_font(FL_HELVETICA, 14);
    fl_draw(styled_node_ref_->ref->value.c_str(),
            x() + 5,
            y() + 14,
            w() - 10,
            h() - 10,
            FL_ALIGN_LEFT | FL_ALIGN_WRAP);
}

HtmlWidget::HtmlWidget(int x, int y, int w, int h, StyledNode *node)
    : Fl_Group(x, y, w, h), styled_node_ref_(node), owned_image_(nullptr) {
}

HtmlWidget::~HtmlWidget() {
    if (owned_image_)
        delete owned_image_;
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
int HtmlWidget::handle(int event) {

    if (Fl_Group::handle(event)) {
        return 1;
    }

    int mx = Fl::event_x();
    int my = Fl::event_y();
    EventState state = {mx, my};

    switch (event) {
        case FL_PUSH:

            if (Fl::event_inside(this) && Fl::event_button() == FL_LEFT_MOUSE) {
                is_dragging_ = true;
                if (window()) {
                    Fl::grab(window());
                }

                if (callback) {
                    callback(this, EventType::Push, state);
                }
                return 1;
            }
            break;

        case FL_DRAG:
            if (is_dragging_) {
                if (callback) {
                    callback(this, EventType::Drag, state);
                }
                return 1;
            }
            break;

        case FL_RELEASE:
            if (is_dragging_) {
                is_dragging_ = false;
                if (window()) {
                    Fl::grab(0);
                }

                if (callback) {
                    callback(this, EventType::Click, state);
                }
                return 1;
            }
            break;

        case FL_ENTER:
            if (callback) {
                callback(this, EventType::MouseEnter, state);
            }
            return 1;

        case FL_LEAVE:
            if (callback) {
                callback(this, EventType::MouseLeave, state);
            }
            return 1;
    }

    return 0;
}
void HtmlWidget::set_callback(HtmlWidget::EventCallback callback) {
    this->callback = callback;
}
std::string HtmlWidget::id() {
    return styled_node_ref_->ref->attributes["id"];
}
std::string HtmlWidget::tag() {
    return styled_node_ref_->ref->name;
}
bool HtmlWidget::has_class(std::string_view class_name) {
    auto classes = this->classes();

    auto result = classes.find(class_name);

    return result != std::string::npos;
}
std::string HtmlWidget::classes() {
    return styled_node_ref_->ref->attributes["class"];
}
