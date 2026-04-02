#pragma once

#include "Layer.h"
#include "LayerStack.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "imguilayer.h"
#include <memory>
#include <type_traits>

struct WindowData {
    int width = 1280;
    int height = 720;
    int pos_x = -1;
    int pos_y = -1;
    std::string title = "Window Title";
};

struct AppData {
    std::string name = "Application";
    WindowData windowdata;
};

class Application {
public:
    Application(const AppData &appdata = AppData());
    ~Application();

    bool Init();
    void Run();
    void Shutdown();

    template <typename TLayer>
        requires(std::is_base_of_v<SakuraVNE::Layer, TLayer>)
    void PushLayer() {
        auto newLayer = std::make_unique<TLayer>();
        SakuraVNE::Layer *layer = newLayer.get();

        m_LayerStack.PushLayer(std::move(newLayer));

        layer->OnAttach();
    }

    template <typename TLayer>
        requires(std::is_base_of_v<SakuraVNE::Layer, TLayer>)
    TLayer *GetLayer() {
        for (const auto &layer : m_LayerStack) {
            if (auto casted = dynamic_cast<TLayer *>(layer.get())) {
                return casted;
            }
        }
        return nullptr;
    }

    template <typename TLayer>
        requires(std::is_base_of_v<SakuraVNE::Layer, TLayer>)
    void PushOverlay() {
        auto newOverlay = std::make_unique<TLayer>();
        SakuraVNE::Layer *layer = newOverlay.get();

        m_LayerStack.PushOverLay(std::move(newOverlay));

        layer->OnAttach();
    }

    inline AppData &GetAppData() { return m_AppData; }
    inline SDL_Window *GetSDLWindow() { return m_Window; }
    inline SDL_Renderer *GetSDLRenderer() { return m_Renderer; }
    inline SDL_Surface *GetSDLWindowSurface() { return m_Surface; }
    inline void SetSDLWindowSurface(SDL_Surface *newSurface) { m_Surface = newSurface; }

    bool &GetRunningState() { return m_isRunning; }
    void SetRunningState(bool isRunning) { m_isRunning = isRunning; }

    static Application &Get() { return *s_Instance; }

private:
    bool m_isRunning;
    bool m_initResult;

    SDL_Window *m_Window;
    SDL_Surface *m_Surface;
    SDL_Renderer *m_Renderer;

    AppData m_AppData;

    SakuraVNE::LayerStack m_LayerStack;
    SakuraVNE::ImGuiLayer *m_ImGuiLayer;

    static Application *s_Instance;
};
