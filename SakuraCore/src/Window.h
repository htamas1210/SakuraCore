#pragma once

#include "Event.h"
#include "SDL3/SDL_video.h"
#include "imgui_impl_sdl3.h"
#include <cstdint>
#include <functional>
#include <string>

namespace SakuraVNE {
struct WindowData {
    std::string title = "Window Title";
    int width = 1280;
    int height = 720;
    int pos_x = -1;
    int pos_y = -1;
    bool isResizable = true;
    bool isVsync = true;

    using EventCallbackFn = std::function<void(Event &)>;
    EventCallbackFn eventCallback;
};

class Window {
public:
    Window(const WindowData &data = WindowData());
    ~Window();

    void Create();
    void Destroy();

    void Update();
    void Resize();
    void ProcessEvent(const SDL_Event &event);

    void RaiseEvent(Event &event);

    bool ShouldClose() const;
    auto GetMousePos() const;

    SDL_Window *GetHandle() const { return m_Handle; }

private:
    WindowData m_Data;
    SDL_Window *m_Handle = nullptr;
    float *m_MouseXPos = nullptr;
    float *m_MouseYPos = nullptr;
};

} // namespace SakuraVNE
