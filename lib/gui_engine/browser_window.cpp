#include "browser_window.hpp"
#include "fltk_renderer.hpp"
#include "layout_engine.hpp"
#include <FL/Fl_Double_Window.H>
#include <iostream>

BrowserWindow::BrowserWindow(int width, int height, std::string_view title) : Fl_Double_Window(width, height, title.data()), fltk_root_(nullptr), layout_root_(nullptr) {
    this->end();

    HtmlWidget::trigger_layout_update = [this]() {
        // Вызываем update_layout с текущими размерами
        this->update_layout(this->w(), this->h());
        this->redraw();
    };
}

void BrowserWindow::set_styled_root(std::unique_ptr<StyledNode> layout_root) {
    layout_root_ = std::move(layout_root);

    update_layout(0, 0);
    int min_w = fltk_root_->child(0)->w();
    int min_h = fltk_root_->child(0)->h();

    update_layout(1000000, 1000000);
    int max_w = fltk_root_->child(0)->w();
    int max_h = fltk_root_->child(0)->h();

    //size_range(min_w, min_h, max_w, max_h);

    update_layout(w(), h());

    if (fltk_root_) { this->resizable(fltk_root_); }
}

void BrowserWindow::update_layout(int width, int height) {
    if (not layout_root_) { return; }

    std::unique_ptr<LayoutBox> new_layout = layout_engine::calculate_layout(layout_root_.get(), width, height);

    if (not fltk_root_) {
        this->begin();
        fltk_root_ = fltk_renderer::create_widget_tree(new_layout.get());
        this->end();
        this->resizable(fltk_root_);

    } else {
        fltk_renderer::update_widget_tree(fltk_root_, new_layout.get());
    }

    if (on_update) {
        on_update();
    }
}

void BrowserWindow::resize(int x, int y, int width, int height) {
    Fl_Double_Window::resize(x, y, width, height);
    update_layout(width, height);
}

HtmlWidget *BrowserWindow::get_root() {
    return this->fltk_root_;
}
int BrowserWindow::handle(int event) {
    int result = Fl_Double_Window::handle(event);

    update_layout(w(), h());
    redraw();

    return result;
}
void BrowserWindow::run() {
    Fl::run();
}
