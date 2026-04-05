#pragma once

#include "Event.h"
#include "Layer.h"
#include "LayerStack.h"
#include "SDL3/SDL_render.h"
#include "SDL3/SDL_video.h"
#include "Window.h"
#include "imguiinit.h"
#include <memory>
#include <type_traits>
#include <vector>

namespace SakuraVNE {

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
    void RaiseEvent(Event &e);

    template <typename TLayer>
        requires(std::is_base_of_v<Layer, TLayer>)
    void PushLayer() {
        auto newLayer = std::make_unique<TLayer>();
        Layer *layer = newLayer.get();

        m_LayerStack.PushLayer(std::move(newLayer));

        layer->OnAttach();
    }

    template <typename TLayer>
        requires(std::is_base_of_v<Layer, TLayer>)
    TLayer *GetLayer() {
        for (const auto &layer : m_LayerStack) {
            if (auto casted = dynamic_cast<TLayer *>(layer.get())) {
                return casted;
            }
        }
        return nullptr;
    }

    template <typename TLayer>
        requires(std::is_base_of_v<Layer, TLayer>)
    TLayer *GetLayer(const std::string &layerName) {
        for (const auto &layer : m_LayerStack) {
            if (auto casted = dynamic_cast<TLayer *>(layer.get()) && layer->GetName() == layerName) {
                return casted;
            }
        }
        return nullptr;
    }

    template <typename TLayer>
        requires(std::is_base_of_v<Layer, TLayer>)
    void PushOverlay() {
        auto newOverlay = std::make_unique<TLayer>();
        Layer *layer = newOverlay.get();

        m_LayerStack.PushOverLay(std::move(newOverlay));

        layer->OnAttach();
    }

    inline AppData &GetAppData() { return m_AppData; }
    inline SDL_Window *GetSDLWindow(const int index = 0) { return m_Window[index]->GetHandle(); }
    inline SDL_Renderer *GetSDLRenderer() { return m_Renderer; }
    inline SDL_Surface *GetSDLWindowSurface() { return m_Surface; }
    inline void SetSDLWindowSurface(SDL_Surface *newSurface) { m_Surface = newSurface; }

    std::shared_ptr<Window> GetWindow(const int index = 0) const { return m_Window[index]; }

    bool &GetRunningState() { return m_isRunning; }
    void SetRunningState(bool isRunning) { m_isRunning = isRunning; }

    static Application &Get() { return *s_Instance; }

private:
    bool m_isRunning;
    bool m_initResult;

    SDL_Surface *m_Surface;
    SDL_Renderer *m_Renderer;

    AppData m_AppData;
    std::vector<std::shared_ptr<Window>> m_Window;

    LayerStack m_LayerStack;
    ImGuiInit *m_ImGui;

    static Application *s_Instance;

    friend class Layer;
};
} // namespace SakuraVNE
