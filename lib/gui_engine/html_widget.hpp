#pragma once
#include "dom_types.hpp"
#include "layout_types.hpp"
#include <FL/Fl_Group.H>
#include <functional>
#include <string>

class HtmlWidget : public Fl_Group {
public:
    bool is_dragging_ = false;

    enum class EventType {
        Push = 0,
        Drag = 1,
        Click = 2,
        MouseEnter = 3,
        MouseLeave = 4
    };
    struct EventState {
        int mx, my;
    };
    using EventCallback = std::function<void(HtmlWidget *, EventType, EventState)>;

    bool is_dragging = false;

private:
    StyledNode *styled_node_ref_;
    Fl_Image *owned_image_;

    Fl_Color parse_color(const std::string &css_color);


    void draw_background();
    void draw_content_text();
    void draw_image();

    EventCallback callback;

public:
    int w() { return Fl_Group::w(); }
    int h() { return Fl_Group::h(); }

    int x() { return Fl_Group::x(); }
    int y() { return Fl_Group::y(); }


    HtmlWidget(int x, int y, int w, int h, StyledNode *node);
    ~HtmlWidget() override;

    int handle(int event) override;

    void set_callback(EventCallback callback);

    void image(Fl_Image *img_to_own);
    void draw() override;

    std::string id();

    std::string classes();

    std::string tag();

    bool has_class(std::string_view class_name);

    void set_style(const std::string &key, std::string value);
    std::string get_style(const std::string &key);

    Fl_Widget *parent() const { return Fl_Group::parent(); }
};