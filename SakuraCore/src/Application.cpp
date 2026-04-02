#include "Application.h"
#include "Event.h"
#include "Layer.h"
#include "Log.h"
#include "SDL3/SDL_main.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_stdinc.h"
#include "SDL3/SDL_timer.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"

#define SDL_MAIN_HANDLED 1

Application *Application::s_Instance = nullptr;

Application::Application(const AppData &appdata) : m_Window(nullptr), m_Renderer(nullptr), m_Surface(nullptr), m_isRunning(false), m_AppData(appdata) {
    SDL_SetMainReady();
    m_initResult = Init();
}
Application::~Application() { Shutdown(); }

bool Application::Init() {
    SetRunningState(true);

    s_Instance = this;

    SakuraVNE::Log::Init();
    LOG_INFO("Initialized logger library");

    LOG_INFO("window width: {0}, height: {1}", GetAppData().windowdata.width, GetAppData().windowdata.height);

    // Init sdl
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == 0) {
        LOG_ERROR("SDL could not be initialized! {0}", SDL_GetError());
        Shutdown();
        return false;
    }

    SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE);
    m_Window = SDL_CreateWindow(GetAppData().windowdata.title.c_str(), GetAppData().windowdata.width, GetAppData().windowdata.height, windowFlags);

    if (!m_Window) {
        LOG_ERROR("SDL window could not be created! {0}", SDL_GetError());
        Shutdown();

        return false;
    } else {
        LOG_INFO("SDl window created");
    }

    if (GetAppData().windowdata.pos_x != -1 && GetAppData().windowdata.pos_y != -1) {
        if (!SDL_SetWindowPosition(m_Window, GetAppData().windowdata.pos_x, GetAppData().windowdata.pos_y)) {
            LOG_ERROR("Failed to set SDL_Window position {0}", SDL_GetError());
        } else {
            LOG_INFO("SDL window position set to the initial value: x {0}, y {1}", GetAppData().windowdata.pos_x, GetAppData().windowdata.pos_y);
        }
    } else {
        LOG_WARN("SDL window position not set. Will not attempt to set window position.");
    }

    LOG_INFO("Available renderer drivers:");
    for (int i = 0; i < SDL_GetNumRenderDrivers(); i++) {
        LOG_INFO("{0}. {1}", i + 1, SDL_GetRenderDriver(i));
    }

    m_Renderer = SDL_CreateRenderer(m_Window, nullptr);
    if (!m_Renderer) {
        LOG_ERROR("Renderer could not be created! {0}", SDL_GetError());
    } else {
        LOG_INFO("SDL renderer created");
        LOG_INFO("Renderer: {0}", SDL_GetRendererName(m_Renderer));
    }

    SDL_SetRenderVSync(m_Renderer, 1);

    // m_ImGuiLayer = new SakuraVNE::ImGuiLayer();
    // PushOverlay(m_ImGuiLayer);

    this->PushOverlay<SakuraVNE::ImGuiLayer>();

    return true;
}

void Application::Run() {
    if (!m_initResult) {
        return;
    }

    ImGuiIO &io = ImGui::GetIO();

    Uint64 oldTime = SDL_GetTicks();

    while (GetRunningState()) {

        Uint64 currentTime = SDL_GetTicks();
        Uint64 time = currentTime - oldTime;
        oldTime = currentTime;

        // Update functions before rendereing
        for (auto &layer : m_LayerStack) {
            layer->OnFrame(time);
        }

        // Events
        // TODO: can i connect my events to sdl events and use their dispatcher?
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);

            if (event.type == SDL_EVENT_QUIT) {
                SetRunningState(false);
                LOG_INFO("Running state: {0}", GetRunningState());
            }

            if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED && event.window.windowID == SDL_GetWindowID(GetSDLWindow())) {
                SetRunningState(false);
            }

            if (event.type == SDL_EVENT_WINDOW_RESIZED) {
                SDL_GetWindowSize(GetSDLWindow(), &GetAppData().windowdata.width, &GetAppData().windowdata.height);

                SetSDLWindowSurface(SDL_GetWindowSurface(GetSDLWindow()));
            }
        }

        // Rendering
        m_ImGuiLayer->Begin();

        SDL_SetRenderScale(m_Renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(m_Renderer, (Uint8)111, (Uint8)232, (Uint8)168, (Uint8)0);
        SDL_RenderClear(m_Renderer);

        for (auto &layer : m_LayerStack) {
            layer->OnImGuiRender();
        }

        m_ImGuiLayer->End();

        SDL_RenderPresent(m_Renderer);
    }
}

void Application::Shutdown() {
    LOG_WARN("Shutting down the application!");

    SDL_DestroyRenderer(m_Renderer);

    SDL_DestroyWindow(m_Window);

    SDL_Quit();
}
