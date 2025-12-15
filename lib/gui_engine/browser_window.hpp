#pragma once

#include "layout_types.hpp"
#include <FL/Fl_Double_Window.H>
#include <functional>
#include <memory>

class BrowserWindow : public Fl_Double_Window {
    std::unique_ptr<StyledNode> layout_root_;
    Fl_Group *fltk_root_;

public:
    void update_layout(int width, int height);
    BrowserWindow(int width, int height, std::string_view title);

    std::function<void()> on_update;

    void set_styled_root(std::unique_ptr<StyledNode> layout_root);

    Fl_Group *get_root();

    int handle(int event) override;

    void resize(int x, int y, int width, int height) final;

    static void run();
};
