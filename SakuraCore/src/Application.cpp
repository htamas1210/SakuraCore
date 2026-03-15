#include "Application.h"
#include "Event.h"
#include "Log.h"
#include "SDL3/SDL_render.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

Application::Application() : m_Window(nullptr), m_Renderer(nullptr), m_Surface(nullptr), m_isRunning(false) {}
Application::~Application() { Shutdown(); }

bool Application::Init() {
    SetRunningState(true);

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

    // Imgui init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForSDLRenderer(m_Window, m_Renderer);
    ImGui_ImplSDLRenderer3_Init(m_Renderer);

    return true;
}

void Application::Run() {
    bool demoWindowShow = false;
    bool showOtherWindow = false;
    ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    ImGuiIO &io = ImGui::GetIO();

    while (GetRunningState()) {
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

        ImGui_ImplSDLRenderer3_NewFrame();
        ImGui_ImplSDL3_NewFrame();
        ImGui::NewFrame();

        if (demoWindowShow) {
            ImGui::ShowDemoWindow(&demoWindowShow);
        }

        // imgui demo window stuff or frame stuff
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello World!"); // creates window and add later stuff to it

        ImGui::Text("text stuff");
        ImGui::Checkbox("Demo checkbox", &showOtherWindow);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
        ImGui::ColorEdit3("clear color edit", (float *)&clearColor);

        if (ImGui::Button("Button"))
            counter++;

        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application avg %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

        if (ImGui::Button("Quit")) {
            SetRunningState(false);
        }

        ImGui::End();

        // Rendering
        ImGui::Render();
        SDL_SetRenderScale(m_Renderer, io.DisplayFramebufferScale.x, io.DisplayFramebufferScale.y);
        SDL_SetRenderDrawColor(m_Renderer, (Uint8)(clearColor.x * 255), (Uint8)(clearColor.y * 255), (Uint8)(clearColor.z * 255), (Uint8)(clearColor.w * 255));
        SDL_RenderClear(m_Renderer);
        ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), m_Renderer);
        SDL_RenderPresent(m_Renderer);
    }
}

void Application::Shutdown() {
    LOG_WARN("Shutting down the application!");

    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();

    SDL_DestroyRenderer(m_Renderer);

    // Destroy window
    SDL_DestroyWindow(m_Window);

    // Quit SDL subsystems
    SDL_Quit();
}
