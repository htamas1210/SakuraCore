#pragma once

#include "SDL3/SDL_stdinc.h"
#include <string>

namespace SakuraVNE {
class Layer {
public:
    Layer(const std::string &name = "Layer", bool isActive = true);
    virtual ~Layer() = default;

    virtual void OnFrame(Uint64 timestamp) {}
    virtual void OnEvent(/*Event &e*/) {}
    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnImGuiRender() {}
    virtual void TransitionTo() {}
    virtual void Suspend() {}
    virtual void Activate() {}

    const std::string &GetName() const { return m_LayerName; }

protected:
    std::string m_LayerName;
    bool m_isActive = true;
};
} // namespace SakuraVNE
