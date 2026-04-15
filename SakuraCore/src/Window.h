#pragma once

#include "Event.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_video.h"
#include <functional>
#include <span>
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

    bool Create(std::span<const SDL_WindowFlags> flags = {});
    void Destroy();

    void Update();
    // TODO: implement
    void AddFlags(std::span<const SDL_WindowFlags> flags = {});
    // TODO: implement
    void RemoveFlags(std::span<const SDL_WindowFlags> flags = {});
    void Resize();
    void ProcessEvent(const SDL_Event &event);

    void RaiseEvent(Event &event);

    bool ShouldClose() const;
    auto GetMousePos() const;

    SDL_Window *GetHandle() const { return m_Handle; }

private:
    WindowData m_Data;
    SDL_Window *m_Handle = nullptr;
    SDL_WindowFlags m_Flags;
    float *m_MouseXPos = nullptr;
    float *m_MouseYPos = nullptr;
};

} // namespace SakuraVNE
