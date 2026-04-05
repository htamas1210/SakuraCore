#include "Window.h"

#include "Application.h"
#include "InputEvents.h"
#include "Log.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_mouse.h"
#include "SDL3/SDL_video.h"
#include "WindowEvents.h"

namespace SakuraVNE {
Window::Window(const WindowData &data) : m_Data(data) {}
Window::~Window() { Destroy(); }

void Window::Create() {
    // TODO: maybe get an unknow amount of parameters / an array of window flags? or just provide a seperate function for it
    SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIGH_PIXEL_DENSITY);
    m_Handle = SDL_CreateWindow(m_Data.title.c_str(), m_Data.width, m_Data.height, windowFlags);

    if (!m_Handle) {
        LOG_ERROR("SDL window could not be created! {0}", SDL_GetError());
        // FIX: there has to be a better way to shutdown this cannot be correct, because i just quit the sdl stuff but the init continues
        Application::Get().Shutdown();
        return;
    } else {
        LOG_INFO("SDl window created");
    }

    if (m_Data.pos_x != -1 && m_Data.pos_y != -1) {
        if (!SDL_SetWindowPosition(m_Handle, m_Data.pos_x, m_Data.pos_y)) {
            LOG_ERROR("Failed to set SDL_Window position {0}", SDL_GetError());
        } else {
            LOG_INFO("SDL window position set to the initial value: x {0}, y {1}", m_Data.pos_x, m_Data.pos_y);
        }
    } else {
        LOG_WARN("SDL window position not set. Will not attempt to set window position.");
    }
}

// TODO: add the window callbacks here as seperate functions because sdl functions that way
// each of them will get a function which will receive a window event filled in with the window's data (id (i guess))
// and then they will dispatch it to the event system using the same raise event

void Window::ProcessEvent(const SDL_Event &event) {
    switch (event.type) {
    case SDL_EVENT_WINDOW_CLOSE_REQUESTED: {
        WindowClosedEvent e;
        RaiseEvent(e);
        break;
    }

    case SDL_EVENT_WINDOW_RESIZED: {
        WindowResizeEvent e((uint32_t)event.window.data1, (uint32_t)event.window.data2);
        RaiseEvent(e);
        break;
    }

    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP: {
        bool isRepeat = event.key.repeat != 0;
        if (event.type == SDL_EVENT_KEY_DOWN) {
            KeyPressedEvent e(event.key.key, isRepeat);
            RaiseEvent(e);
        } else {
            KeyReleasedEvent e(event.key.key);
            RaiseEvent(e);
        }
        break;
    }

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP: {
        if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            MouseButtonPressedEvent e(event.button.button);
            RaiseEvent(e);
        } else {
            MouseButtonReleasedEvent e(event.button.button);
            RaiseEvent(e);
        }
        break;
    }

    case SDL_EVENT_MOUSE_MOTION: {
        MouseMovedEvent e(event.motion.x, event.motion.y);
        RaiseEvent(e);
        break;
    }

    case SDL_EVENT_MOUSE_WHEEL: {
        MouseScrolledEvent e(event.wheel.x, event.wheel.y);
        RaiseEvent(e);
        break;
    }
    }
}

void Window::Resize() {
    SDL_GetWindowSize(m_Handle, &m_Data.width, &m_Data.height);
    SDL_SetWindowSize(m_Handle, m_Data.width, m_Data.height);
}

void Window::Destroy() {
    if (m_Handle) {
        SDL_DestroyWindow(m_Handle);
    }
    m_Handle = nullptr;
}

void Window::RaiseEvent(Event &event) {
    if (m_Data.eventCallback) {
        m_Data.eventCallback(event);
    }
}

// TODO: this need testing because i am really not sure this is correct
// should this be static or go somewhere else | probably in application?
auto Window::GetMousePos() const {
    struct result {
        float *x;
        float *y;
    };

    SDL_GetMouseState(m_MouseXPos, m_MouseYPos);

    return result{m_MouseXPos, m_MouseYPos};
}
} // namespace SakuraVNE
