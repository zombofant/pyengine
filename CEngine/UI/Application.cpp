#include "Application.hpp"

#include "CEngine/GL/Base.hpp"
#include "CEngine/GL/CairoUtils.hpp"

namespace PyEngine { namespace UI {

using namespace PyEngine::GL;
using namespace UI;

inline unsigned int make_pot(unsigned int v)
{
    // From http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
    // Credit: Sean Anderson
    v -= 1;
    v |= v >> 1;
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    return v + 1;
}

/* PyEngine::Application */

Application::Application():
    RootWidget(),
    _log(log->getChannel("ui")),
    _cairo_tex(0),
    _cairo_tex_w(0),
    _cairo_tex_h(0),
    _cairo_tex_s(0),
    _cairo_tex_t(0)
{

}

Application::~Application()
{
    if (_cairo_tex) {
        glDeleteTextures(1, &_cairo_tex);
    }
}

void Application::_recreate_cairo_surface(
    unsigned int width, unsigned int height)
{
    RootWidget::_recreate_cairo_surface(width, height);

    unsigned int pot_w = make_pot(width);
    unsigned int pot_h = make_pot(height);

    _cairo_tex_s = (float)width / pot_w;
    _cairo_tex_t = (float)height / pot_h;

    if (_cairo_tex == 0) {
        glGenTextures(1, &_cairo_tex);
    }

    if ((pot_w != _cairo_tex_w) || (pot_h != _cairo_tex_h)) {
        glBindTexture(GL_TEXTURE_2D, _cairo_tex);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            pot_w,
            pot_h,
            0,
            GL_LUMINANCE,
            GL_UNSIGNED_BYTE,
            nullptr);
        raiseLastGLError();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        raiseLastGLError();
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
        raiseLastGLError();
        glBindTexture(GL_TEXTURE_2D, 0);

        _cairo_tex_w = pot_w;
        _cairo_tex_h = pot_h;
    }

}

void Application::init_display(
    Display &dpy,
    const coord_int_pair_t &dimensions,
    bool fullscreen,
    DisplayMode *suggestion)
{
    std::vector<DisplayMode> modes(dpy.getDisplayModes());
    std::sort(modes.begin(), modes.end());

    if (modes.size() == 0) {
        throw std::runtime_error("No display modes found");
    }

    std::unique_ptr<DisplayMode> candidate(
        select_display_mode(modes, suggestion));

    _display = &dpy;

    _log->log(Information) << "Creating window with display mode "
        << *candidate << submit;

    _window = dpy.createWindow(*candidate,
                               dimensions.first,
                               dimensions.second,
                               fullscreen);
    _window->switchTo();
    _window->initializeGLEW();

    modeswitch(dimensions, fullscreen);

    glClearColor(0, 0.05, 0.1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _window->flip();
}

void Application::modeswitch(const coord_int_pair_t &dimensions,
                             bool fullscreen)
{
    const auto &screens = _display->getScreens();
    if (fullscreen) {
        _window->setFullscreen(
            screens[0].index,
            screens[0].index,
            screens[0].index,
            screens[0].index);
    } else {
        _window->setWindowed(
            screens[0].index,
            dimensions.first,
            dimensions.second);
    }

    absolute_rect() = Rect(0, 0, dimensions.first, dimensions.second);
    realign();
}

std::unique_ptr<DisplayMode> Application::select_display_mode(
    const std::vector<DisplayMode> &modes,
    DisplayMode *suggestion)
{
    std::unique_ptr<DisplayMode> candidate(new DisplayMode());

    if (suggestion) {
        *candidate = *suggestion;
    } else {
        *candidate = modes.back();
    }

    if (std::find(modes.cbegin(), modes.cend(), *candidate) == modes.cend()) {
        _log->log(Debug) << "No display mode " << *candidate
                                << " available..." << submit;
        candidate->samples = 0;
    }

    if (std::find(modes.cbegin(), modes.cend(), *candidate) == modes.cend()) {
        _log->log(Debug) << "No display mode " << *candidate
                                << " available..." << submit;
        candidate->stencilBits = 0;
    }


    if (std::find(modes.cbegin(), modes.cend(), *candidate) == modes.cend()) {
        *candidate = modes.back();
        _log->log(Warning) << "Could not find a matching display mode. "
                                  << "Using best available: " << *candidate
                                  << submit;
    }

    return candidate;
}

void Application::ui_frame_unsynced(TimeFloat deltaT)
{
    Rect &rect = absolute_rect();
    glViewport(0, 0, rect.get_width(), rect.get_height());
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(rect.get_x(), rect.get_width(), rect.get_height(), rect.get_y(), -1, 1);
    glMatrixMode(GL_MODELVIEW);

    render();

    glBindTexture(GL_TEXTURE_2D, _cairo_tex);
    if (get_surface_dirty() || true) {
        glTexCairoSurfaceSubImage2D(
            GL_TEXTURE_2D, 0,
            0, 0,
            get_cairo_surface());
    }

    float s = _cairo_tex_s,
          t = _cairo_tex_t;

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);
        glVertex2f(0, 0);

        glTexCoord2f(0, t);
        glVertex2f(0, rect.get_height());

        glTexCoord2f(s, t);
        glVertex2f(rect.get_width(), rect.get_height());

        glTexCoord2f(s, 0);
        glVertex2f(rect.get_width(), 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Application::frame_unsynced(TimeFloat deltaT)
{
    _window->switchTo();


}

}
}
