#pragma once

#include <string>

namespace SakuraVNE {
class Layer {
public:
    Layer(const std::string &name = "Layer");
    virtual ~Layer() = default;

    virtual void OnStart() {}
    virtual void OnFrame(float timestamp) {}
    virtual void OnEnd() {}
    virtual void OnEvent() {}
    virtual void OnAttach() {}
    virtual void OnDetach() {}

    const std::string &GetName() const { return m_LayerName; }

protected:
    std::string m_LayerName;
};
} // namespace SakuraVNE
