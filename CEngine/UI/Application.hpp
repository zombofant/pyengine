#ifndef _PYE_APPLICATION_H
#define _PYE_APPLICATION_H

#include "CEngine/WindowInterface/Display.hpp"
#include "CEngine/WindowInterface/Window.hpp"
#include "CEngine/WindowInterface/EventLoop.hpp"
#include "CEngine/IO/Log.hpp"

#include "Widgets/RootWidget.hpp"

namespace PyEngine { namespace UI {

class Application: public RootWidget
{
public:
    Application();
    ~Application() override;

private:
    LogChannelHandle _log;

    GLuint _cairo_tex;
    unsigned int _cairo_tex_w;
    unsigned int _cairo_tex_h;
    float _cairo_tex_s;
    float _cairo_tex_t;

protected:
    PyEngine::Display *_display;
    PyEngine::WindowHandle _window;

protected:
    void _recreate_cairo_surface(unsigned int width,
                                 unsigned int height) override;

protected:
    void init_display(
        Display &dpy,
        const coord_int_pair_t &dimensions = std::make_pair(800, 600),
        bool fullscreen = false,
        DisplayMode *suggestion = nullptr);

    void modeswitch(
        const coord_int_pair_t &dimensions = std::make_pair(800, 600),
        bool fullscreen = false);

    virtual void ui_frame_unsynced(TimeFloat deltaT);

    virtual std::unique_ptr<DisplayMode> select_display_mode(
        const std::vector<DisplayMode> &modes,
        DisplayMode *suggestion = nullptr);

public:
    void frame_unsynced(TimeFloat deltaT) override;

};

}
}

#endif
