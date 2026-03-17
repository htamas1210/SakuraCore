#pragma once

#include "Layer.h"

namespace SakuraVNE {
class ImGuiLayer : public Layer {
public:
    ImGuiLayer();
    ~ImGuiLayer() = default;

    virtual void OnFrame(Uint64 timestamp) override;
    virtual void OnAttach() override;
    virtual void OnDetach() override;
    virtual void OnImGuiRender() override;
    virtual void OnEvent() override;

    void Begin();
    void End();
};
} // namespace SakuraVNE
