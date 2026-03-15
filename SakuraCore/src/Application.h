#pragma once

#include "SDL3/SDL.h"

struct WindowData {
    int width = 1280;
    int height = 720;
    int pos_x = -1;
    int pos_y = -1;
    const char *title = "Sakura Visual Novel Engine";
};

class Application {
public:
    Application();
    ~Application();

    bool Init();
    void Run();
    void Shutdown();

    inline WindowData &GetWindowData() { return m_WindowData; }
    inline SDL_Window *GetSDLWindow() { return m_Window; }
    inline SDL_Surface *GetSDLWindowSurface() { return m_Surface; }
    inline void SetSDLWindowSurface(SDL_Surface *newSurface) { m_Surface = newSurface; }

    bool &GetRunningState() { return m_isRunning; }
    void SetRunningState(bool isRunning) { m_isRunning = isRunning; }

private:
    SDL_Window *m_Window;
    SDL_Surface *m_Surface;
    SDL_Renderer *m_Renderer;

    WindowData m_WindowData;

    bool m_isRunning;
};
