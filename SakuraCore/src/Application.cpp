#include "Application.h"
#include "Event.h"
#include "Layer.h"
#include "Log.h"
#include "SDL3/SDL_events.h"
#include "SDL3/SDL_hints.h"
#include "SDL3/SDL_init.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_stdinc.h"
#include "SDL3/SDL_timer.h"
#include "SDL3/SDL_video.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imguiinit.h"
#include <memory>
#include <ranges>

namespace SakuraVNE {
Application *Application::s_Instance = nullptr;

Application::Application(const AppData &appdata) : m_Window(), m_Renderer(nullptr), m_Surface(nullptr), m_isRunning(false), m_AppData(appdata) { m_initResult = Init(); }
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

    LOG_INFO("Available renderer drivers:");
    for (int i = 0; i < SDL_GetNumRenderDrivers(); i++) {
        LOG_INFO("{0}. {1}", i + 1, SDL_GetRenderDriver(i));
    }

    // TODO: check here if m_Window is not null otherwise shutdown procedure
    if (m_AppData.windowdata.title.empty()) {
        m_AppData.windowdata.title = m_AppData.name;
    }

    m_AppData.windowdata.eventCallback = [this](Event &event) { RaiseEvent(event); };
    m_Window.push_back(std::make_shared<Window>(m_AppData.windowdata));
    bool isSuccessful = m_Window[0]->Create({m_AppData.windowdata.windowFlags});
    if (!isSuccessful) {
        Shutdown();
        return false;
    }

    m_Renderer = SDL_CreateRenderer(GetSDLWindow(), nullptr);
    if (!m_Renderer) {
        LOG_ERROR("Renderer could not be created! {0}", SDL_GetError());
    } else {
        LOG_INFO("SDL renderer created");
        LOG_INFO("Renderer: {0}", SDL_GetRendererName(m_Renderer));
    }

    SDL_SetRenderVSync(m_Renderer, m_AppData.windowdata.isVsync);

    m_ImGui = new SakuraVNE::ImGuiInit();

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
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL3_ProcessEvent(&event);

            if (event.type == SDL_EVENT_QUIT) {
                SetRunningState(false);
                LOG_INFO("Running state: {0}", GetRunningState());
            }

            SDL_WindowID targetWindowID = 0;

            if (event.type >= SDL_EVENT_WINDOW_FIRST && event.type <= SDL_EVENT_WINDOW_LAST) {
                targetWindowID = event.window.windowID;
            } else if (event.type >= SDL_EVENT_KEY_DOWN && event.type <= SDL_EVENT_KEY_UP) {
                targetWindowID = event.key.windowID;
            } else if (event.type >= SDL_EVENT_MOUSE_MOTION && event.type <= SDL_EVENT_MOUSE_WHEEL) {
                targetWindowID = event.motion.windowID;
            }

            if (targetWindowID != 0) {
                for (auto it = m_Window.begin(); it != m_Window.end();) {

                    if (SDL_GetWindowID((*it)->GetHandle()) == targetWindowID) {
                        (*it)->ProcessEvent(event);

                        if (event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) {
                            it = m_Window.erase(it);

                            if (m_Window.empty()) {
                                SetRunningState(false);
                            }
                        } else {
                            ++it;
                        }
                        break;
                    } else {
                        ++it;
                    }
                }
            }
        }

        if (!m_isRunning) {
            break;
        }

        // Rendering
        for (auto &layer : m_LayerStack) {
            layer->OnRender();
        }

        m_ImGui->Begin();

        SDL_SetRenderScale(m_Renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(m_Renderer, (Uint8)111, (Uint8)232, (Uint8)168, (Uint8)255);
        SDL_RenderClear(m_Renderer);

        for (auto &layer : m_LayerStack) {
            layer->OnImGuiRender();
        }

        m_ImGui->End();

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }

        SDL_RenderPresent(m_Renderer);

        // handle layercommands here after each frame
        m_LayerStack.ProcessCommands();
    }
}

void Application::Shutdown() {
    LOG_WARN("Shutting down the application!");

    SDL_DestroyRenderer(m_Renderer);

    m_Window.clear();

    SDL_Quit();

    s_Instance = nullptr;
}

void Application::RaiseEvent(Event &event) {
    for (auto &layer : std::views::reverse(m_LayerStack)) {
        layer->OnEvent(event);
        if (event.Handled) {
            break;
        }
    }
}
} // namespace SakuraVNE
