#pragma once
#include "dom_types.hpp"
#include "layout_types.hpp"
#include <FL/Fl_Group.H>
#include <functional>
#include <map>
#include <memory>
#include <string>
#include <vector>


class HtmlWidget;

struct DOMEvent {
    std::string type;
    HtmlWidget *target;
    HtmlWidget *current_target;
    int client_x;
    int client_y;
    bool propagation_stopped = false;
    bool default_prevented = false;

    void stop_propagation() { propagation_stopped = true; }
    void prevent_default() { default_prevented = true; }
};

using EventListener = std::function<void(DOMEvent &)>;

class HtmlWidget : public Fl_Group {
private:
    HtmlWidget *logical_parent_ = nullptr;

    StyledNode *styled_node_ref_;
    Fl_Image *owned_image_;

    using Fl_Group::remove;

    std::map<std::string, std::vector<EventListener>> listeners_;

    int get_int_style(const std::string &key, int default_val = 0);
    float get_float_style(const std::string &key, float default_val = 0.0f);


    Fl_Color parse_color(const std::string &css_color);

    void draw_rounded_rect(int x, int y, int w, int h, int radius, Fl_Color color, bool filled, int line_width = 1);
    void draw_background();
    void draw_content_text();
    void draw_image();

public:
    void set_logical_parent(HtmlWidget *parent);

    bool is_pressed_ = false;


    int w() const { return Fl_Group::w(); }
    int h() const { return Fl_Group::h(); }
    int x() const { return Fl_Group::x(); }
    int y() const { return Fl_Group::y(); }

    HtmlWidget(int x, int y, int w, int h, StyledNode *node);
    ~HtmlWidget() override;


    int handle(int event) override;
    void draw() override;


    void add_event_listener(const std::string &type, EventListener listener);


    bool dispatch_event(DOMEvent &event);

    void image(Fl_Image *img_to_own);

    std::string id();
    std::string classes();
    std::string tag();
    bool has_class(std::string_view class_name);

    void set_style(const std::string &key, std::string value);
    std::string get_style(const std::string &key);

    void set_styled_node(StyledNode *node);

    StyledNode *get_node() const { return styled_node_ref_; }
    Fl_Widget *parent() const;


    void append_child(HtmlWidget *child);


    static std::function<void()> trigger_layout_update;
};