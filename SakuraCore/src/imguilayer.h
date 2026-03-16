#pragma once

#include "Layer.h"

namespace SakuraVNE {
class ImGuiLayer : public Layer {
public:
    ImGuiLayer();
    ~ImGuiLayer() = default;

    // virtual void OnStart() override;
    // virtual void OnFrame(float timestamp) override;
    // virtual void OnEnd() override;
    virtual void OnAttach() override;
    virtual void OnDetach() override;
    // virtual void OnEvent() override;

    void Begin();
    void End();
};
} // namespace SakuraVNE
