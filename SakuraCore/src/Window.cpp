#include "Window.h"
#include "InputEvents.h"
#include "Log.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_mouse.h"
#include "SDL3/SDL_video.h"
#include "WindowEvents.h"
#include <span>

namespace SakuraVNE {
Window::Window(const WindowData &data) : m_Data(data) {}
Window::~Window() { Destroy(); }

bool Window::Create(const SDL_WindowFlags flags) {
    m_Flags |= flags;

    m_Handle = SDL_CreateWindow(m_Data.title.c_str(), m_Data.width, m_Data.height, m_Flags);

    if (!m_Handle) {
        LOG_ERROR("SDL window could not be created! {0}", SDL_GetError());
        return false;
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

    return true;
}

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

void Window::AddFlags(std::span<const SDL_WindowFlags> flags) {
    for (auto &flag : flags) {
        switch (flag) {
        case SDL_WINDOW_FULLSCREEN:
            SDL_SetWindowFullscreen(m_Handle, true);
            break;
        case SDL_WINDOW_HIDDEN:
            SDL_ShowWindow(m_Handle);
            break;
        case SDL_WINDOW_BORDERLESS:
            SDL_SetWindowBordered(m_Handle, false);
            break;
        case SDL_WINDOW_RESIZABLE:
            SDL_SetWindowResizable(m_Handle, true);
            break;
        case SDL_WINDOW_MINIMIZED:
            SDL_MinimizeWindow(m_Handle);
            break;
        case SDL_WINDOW_MAXIMIZED:
            SDL_MaximizeWindow(m_Handle);
            break;
        case SDL_WINDOW_MOUSE_GRABBED:
            SDL_SetWindowMouseGrab(m_Handle, true);
            break;
        case SDL_WINDOW_MOUSE_CAPTURE:
            SDL_SetWindowMouseGrab(m_Handle, true);
            break;
        case SDL_WINDOW_MOUSE_RELATIVE_MODE:
            SDL_SetWindowRelativeMouseMode(m_Handle, true);
            break;
        case SDL_WINDOW_MODAL:
            SDL_SetWindowModal(m_Handle, true);
            break;
        case SDL_WINDOW_ALWAYS_ON_TOP:
            SDL_SetWindowAlwaysOnTop(m_Handle, true);
            break;
        case SDL_WINDOW_KEYBOARD_GRABBED:
            SDL_SetWindowKeyboardGrab(m_Handle, true);
            break;
        case SDL_WINDOW_NOT_FOCUSABLE:
            SDL_SetWindowFocusable(m_Handle, false);
            break;
        default:
            LOG_WARN("You can only use this when creating a window or the flag is readonly");
            break;
        }

        m_Flags |= flag;
    }
}

void Window::RemoveFlags(std::span<const SDL_WindowFlags> flags) {
    for (auto &flag : flags) {
        switch (flag) {
        case SDL_WINDOW_FULLSCREEN:
            SDL_SetWindowFullscreen(m_Handle, false);
            break;
        case SDL_WINDOW_HIDDEN:
            SDL_HideWindow(m_Handle);
            break;
        case SDL_WINDOW_BORDERLESS:
            SDL_SetWindowBordered(m_Handle, true);
            break;
        case SDL_WINDOW_RESIZABLE:
            SDL_SetWindowResizable(m_Handle, false);
            break;
        case SDL_WINDOW_MINIMIZED:
        case SDL_WINDOW_MAXIMIZED:
            SDL_RestoreWindow(m_Handle);
            break;
        case SDL_WINDOW_MOUSE_GRABBED:
            SDL_SetWindowMouseGrab(m_Handle, false);
            break;
        case SDL_WINDOW_MOUSE_CAPTURE:
            SDL_SetWindowMouseGrab(m_Handle, false);
            break;
        case SDL_WINDOW_MOUSE_RELATIVE_MODE:
            SDL_SetWindowRelativeMouseMode(m_Handle, false);
            break;
        case SDL_WINDOW_MODAL:
            SDL_SetWindowModal(m_Handle, false);
            break;
        case SDL_WINDOW_ALWAYS_ON_TOP:
            SDL_SetWindowAlwaysOnTop(m_Handle, false);
            break;
        case SDL_WINDOW_KEYBOARD_GRABBED:
            SDL_SetWindowKeyboardGrab(m_Handle, false);
            break;
        case SDL_WINDOW_NOT_FOCUSABLE:
            SDL_SetWindowFocusable(m_Handle, true);
            break;
        default:
            LOG_WARN("You can only use this when creating a window or the flag is readonly");
            break;
        }

        m_Flags |= flag;
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
