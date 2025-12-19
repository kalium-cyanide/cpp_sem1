#include "fltk_renderer.hpp"
#include "html_widget.hpp"

#include <FL/Fl_Box.H>
#include <FL/Fl_Button.H>
#include <FL/Fl_Group.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_JPEG_Image.H>
#include <FL/Fl_PNG_Image.H>
#include <FL/Fl_RGB_Image.H>
#include <FL/Fl_SVG_Image.H>
#include <FL/fl_draw.H>

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <map>
#include <string>

namespace {

    Fl_Color ParseFltkColor(const std::string &css_color) {
        if (css_color.empty()) return (FL_BLACK);

        unsigned int r = 0, g = 0, b = 0;

        if (css_color[0] == '#' and css_color.length() == 7) {
            if (sscanf(css_color.c_str() + 1, "%02x%02x%02x", &r, &g, &b) == 3) {
                return fl_rgb_color(r, g, b);
            }
        } else if (css_color.rfind("rgb(", 0) == 0) {
            if (sscanf(css_color.c_str(), "rgb(%u,%u,%u)", &r, &g, &b) == 3) {
                return fl_rgb_color(r, g, b);
            }
        } else {
            static const std::map<std::string, Fl_Color> kColorMap = {
                    {"black", FL_BLACK},
                    {"white", FL_WHITE},
                    {"red", FL_RED},
                    {"green", FL_GREEN},
                    {"blue", FL_BLUE},
                    {"yellow", FL_YELLOW},
                    {"gray", FL_GRAY}};
            auto it = kColorMap.find(css_color);
            if (it != kColorMap.end()) {
                return it->second;
            }
        }
        return FL_BLACK;
    }

    HtmlWidget *CreateWidgetRecursive(const LayoutBox *box) {
        if (!box) return nullptr;

        StyledNode *node = box->styled_node_ref;
        if (!node) return nullptr;

        int x = static_cast<int>(box->x);
        int y = static_cast<int>(box->y);
        int w = static_cast<int>(box->width);
        int h = static_cast<int>(box->height);

        HtmlWidget *widget = nullptr;

        if (node->name == "img") {
            auto src = node->ref->attributes["src"];

            Fl_Image *loaded_img = nullptr;

            if (!src.empty()) {
                if (src.ends_with(".svg")) {
                    loaded_img = new Fl_SVG_Image(src.c_str());
                } else if (src.ends_with(".png")) {
                    loaded_img = new Fl_PNG_Image(src.c_str());
                } else if (src.ends_with(".jpg") || src.ends_with(".jpeg")) {
                    loaded_img = new Fl_JPEG_Image(src.c_str());
                }
            }

            auto *wi = new HtmlWidget(x, y, w, h, node);


            if (loaded_img and loaded_img->w() > 0 and loaded_img->h() > 0) {
                wi->image(loaded_img);
            } else {
                delete loaded_img;
            }

            widget = wi;

        } else {
            widget = new HtmlWidget(x, y, w, h, node);
        }


        if (auto *group = dynamic_cast<Fl_Group *>(widget)) {
            group->begin();
            for (const auto &child_box: box->children) {
                CreateWidgetRecursive(child_box.get());
            }
            group->end();
        }


        return widget;
    }


    struct WidgetZOrder {
        Fl_Widget *widget;
        int z_index;
        int original_index;
    };

    void update_recursive(Fl_Group *parentWidget, const LayoutBox *parentLayout) {


        int fltk_children_count = parentWidget->children();

        for (size_t i = 0; i < parentLayout->children.size(); ++i) {


            if (i >= fltk_children_count) break;

            Fl_Widget *childBase = parentWidget->child(i);
            const LayoutBox *childLayout = parentLayout->children[i].get();


            if (auto *htmlChild = dynamic_cast<HtmlWidget *>(childBase)) {
                htmlChild->set_styled_node(childLayout->styled_node_ref);
            }


            bool is_absolute = childLayout->styled_node_ref->styles["position"] == "absolute";

            if (childLayout->styled_node_ref->styles["display"] == "none") {
                childBase->hide();
            } else {
                childBase->show();
            }

            childBase->resize(
                    (int) childLayout->x + (is_absolute ? 0 : parentWidget->x()),
                    (int) childLayout->y + (is_absolute ? 0 : parentWidget->y()),
                    (int) childLayout->width,
                    (int) childLayout->height);

            if (auto *group = dynamic_cast<Fl_Group *>(childBase)) {
                update_recursive(group, childLayout);
            }
        }


        std::vector<WidgetZOrder> items;
        items.reserve(parentWidget->children());

        for (int i = 0; i < parentWidget->children(); ++i) {
            Fl_Widget *w = parentWidget->child(i);
            int z = 0;
            if (auto *hw = dynamic_cast<HtmlWidget *>(w)) {

                auto zi = hw->get_style("z-index");
                z = zi == "" ? 0 : std::stoi(zi);
            }
            items.push_back({w, z, i});
        }


        std::stable_sort(items.begin(), items.end(), [](const WidgetZOrder &a, const WidgetZOrder &b) {
            if (a.z_index != b.z_index) {
                return a.z_index < b.z_index;
            }
            return a.original_index < b.original_index;
        });


        for (const auto &item: items) {
            parentWidget->remove(item.widget);
        }

        for (const auto &item: items) {
            parentWidget->add(item.widget);
        }

        parentWidget->init_sizes();
    }

}// namespace

namespace fltk_renderer {

    void update_widget_tree(HtmlWidget *rootWidget, const LayoutBox *newLayoutRoot) {
        if (!rootWidget || !newLayoutRoot) { return; }


        rootWidget->resize(
                (int) newLayoutRoot->x,
                (int) newLayoutRoot->y,
                (int) newLayoutRoot->width,
                (int) newLayoutRoot->height);

        update_recursive(rootWidget, newLayoutRoot);
    }

    HtmlWidget *create_widget_tree(const LayoutBox *root_box) {
        if (!root_box) return nullptr;

        return CreateWidgetRecursive(root_box);
    }

}// namespace fltk_renderer