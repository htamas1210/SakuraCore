#pragma once

#include "Layer.h"
#include <vector>

namespace SakuraVNE {
class LayerStack {
public:
    LayerStack() = default;
    ~LayerStack();

    void PushLayer(Layer *layer);
    void PopLayer(Layer *layer);
    void PushOverLay(Layer *layer);
    void PopOverlay(Layer *layer);

#ifdef DEBUG
    // this is only used for the tests for now, so it will be taken out of the release build
    inline const std::vector<Layer *> &GetLayers() const { return m_LayerStack; }
#endif
private:
    std::vector<Layer *> m_LayerStack;
    unsigned int m_LayerIndex = 0;
};
} // namespace SakuraVNE
