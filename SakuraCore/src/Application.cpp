#include "Application.h"
#include "Event.h"
#include "Layer.h"
#include "Log.h"
#include "SDL3/SDL_main.h"
#include "SDL3/SDL_render.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

#define SDL_MAIN_HANDLED 1

Application *Application::s_Instance = nullptr;

Application::Application() : m_Window(nullptr), m_Renderer(nullptr), m_Surface(nullptr), m_isRunning(false) { SDL_SetMainReady(); }
Application::~Application() { Shutdown(); }

bool Application::Init() {
    SetRunningState(true);

    s_Instance = this;

    SakuraVNE::Log::Init();
    LOG_INFO("Initialized logger library");

    LOG_INFO("window width: {0}, height: {1}", GetWindowData().width, GetWindowData().height);

    // Init sdl
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) == 0) {
        LOG_ERROR("SDL could not be initialized! {0}", SDL_GetError());
        Shutdown();
        return false;
    }

    SDL_WindowFlags windowFlags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE);
    m_Window = SDL_CreateWindow(GetWindowData().title, GetWindowData().width, GetWindowData().height, windowFlags);

    if (!m_Window) {
        LOG_ERROR("SDL window could not be created! {0}", SDL_GetError());
        Shutdown();

        return false;
    } else {
        LOG_INFO("SDl window created");
    }

    if (GetWindowData().pos_x != -1 && GetWindowData().pos_y != -1) {
        if (!SDL_SetWindowPosition(m_Window, GetWindowData().pos_x, GetWindowData().pos_y)) {
            LOG_ERROR("Failed to set SDL_Window position {0}", SDL_GetError());
        } else {
            LOG_INFO("SDL window position set to the initial value: x {0}, y {1}", GetWindowData().pos_x, GetWindowData().pos_y);
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

    m_ImGuiLayer = new SakuraVNE::ImGuiLayer();
    PushOverlay(m_ImGuiLayer);

    return true;
}

void Application::Run() {
    ImGuiIO &io = ImGui::GetIO();

    while (GetRunningState()) {

        float time = 0;

        for (auto layer : m_LayerStack) {
            layer->OnFrame(time);
        }

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
                SDL_GetWindowSize(GetSDLWindow(), &GetWindowData().width, &GetWindowData().height);

                SetSDLWindowSurface(SDL_GetWindowSurface(GetSDLWindow()));
            }
        }
        //
        // Rendering
        ImGui::Render();
        SDL_SetRenderScale(m_Renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(m_Renderer, (Uint8)255, (Uint8)255, (Uint8)255, (Uint8)255);
        SDL_RenderClear(m_Renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_Renderer);
        SDL_RenderPresent(m_Renderer);
    }
}

void Application::Shutdown() {
    LOG_WARN("Shutting down the application!");

    SDL_DestroyRenderer(m_Renderer);

    // Destroy window
    SDL_DestroyWindow(m_Window);

    // Quit SDL subsystems
    SDL_Quit();
}

void Application::PushLayer(SakuraVNE::Layer *layer) {
    m_LayerStack.PushLayer(layer);
    layer->OnAttach();
}
void Application::PushOverlay(SakuraVNE::Layer *layer) {
    m_LayerStack.PushOverLay(layer);
    layer->OnAttach();
}
