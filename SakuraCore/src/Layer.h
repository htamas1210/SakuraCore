#pragma once

#include <string>

namespace SakuraVNE {
class Layer {
public:
    Layer(const std::string &name = "Layer");
    virtual ~Layer() = default;

    virtual void OnFrame(float timestamp) {}
    virtual void OnEvent() {}
    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnImGuiRender() {}

    const std::string &GetName() const { return m_LayerName; }

protected:
    std::string m_LayerName;
};
} // namespace SakuraVNE
