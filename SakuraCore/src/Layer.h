#pragma once

#include "Event.h"
#include "SDL3/SDL_stdinc.h"
#include <concepts>
#include <memory>
#include <string>

namespace SakuraVNE {
class Layer {
public:
    Layer(const std::string &name = "Layer", bool isActive = true);
    virtual ~Layer() = default;

    virtual void OnFrame(Uint64 timestamp) {}
    virtual void OnEvent(Event &e) {}
    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnImGuiRender() {}
    virtual void Suspend() { m_isActive = false; }
    virtual void Activate() { m_isActive = true; }

    template <std::derived_from<Layer> T, typename... Args> void TransitionTo(Args &&...args) { QueueTransition(std::move(std::make_unique<T>(std::forward<Args>(args)...))); }

    const std::string &GetName() const { return m_LayerName; }

protected:
    std::string m_LayerName;
    bool m_isActive = true;

private:
    void QueueTransition(std::unique_ptr<Layer> layer);
};
} // namespace SakuraVNE
