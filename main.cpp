#include "FL/Fl_Button.H"
#include "FL/Fl_Hor_Slider.H"
#include "FL/Fl_Slider.H"
#include "FL/Fl_Valuator.H"
#include "FL/Fl_Widget.H"
#include <FL/Fl.H>
#include <FL/Fl_Box.H>
#include <FL/Fl_Window.H>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <memory>
#include <sys/stat.h>

#define ptr(x) std::shared_ptr<decltype(x)>(new x)

struct UI_Info
{
    int w, h;
    double slider_value;
    bool is_horizantal;

    std::shared_ptr<Fl_Window> window_p;

    UI_Info(int w, int h, double slider_value, bool is_horizantal, std::shared_ptr<Fl_Window> window_p)
        : w(w), h(h), slider_value(slider_value), is_horizantal(is_horizantal), window_p(window_p)
    {}
};

int main(int argc, char** argv)
{
    auto window = ptr(Fl_Window(340, 180));
    auto ui_info = ptr(UI_Info(340, 180, 0., true, window));
    //
    auto slider = ptr(Fl_Hor_Slider(0, 0, 100, 40, "SLIDER"));

    slider->callback(
        [](Fl_Widget* w, void* p)
        {
            auto states = static_cast<UI_Info*>(p);
            states->slider_value = dynamic_cast<Fl_Valuator*>(w)->value();

            if (states->is_horizantal)
            {
                states->window_p->size(states->w * (states->slider_value + 1), states->h);
            }
            else
            {
                states->window_p->size(states->w, states->h * (states->slider_value + 1));
            }
        },
        &*ui_info);
    //
    auto button = ptr(Fl_Button(200, 0, 20, 20, "H"));

    button->callback(
        [](Fl_Widget* w, void* p)
        {
            auto state = static_cast<UI_Info*>(p);
            auto button = dynamic_cast<Fl_Button*>(w);
            state->is_horizantal = !state->is_horizantal;

            dynamic_cast<Fl_Button*>(w)->label(state->is_horizantal ? "H" : "W");
        },
        &*ui_info);
    //

    window->end();
    window->show(argc, argv);
    return Fl::run();
}