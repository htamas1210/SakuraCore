#pragma once

#include "Layer.h"
#include <iterator>
#include <memory>
#include <vector>

namespace SakuraVNE {
enum class LayerAction { Push, Pop, Transition };

struct LayerCommand {
    LayerAction action;
    Layer *targetLayer;
    std::unique_ptr<Layer> newLayer;
};

class LayerStack {
public:
    LayerStack();
    ~LayerStack();

    void PushLayer(std::unique_ptr<Layer> layer);
    void PopLayer(Layer *layer);
    void PushOverLay(std::unique_ptr<Layer> layer);
    void PopOverlay(Layer *layer);

    std::vector<std::unique_ptr<Layer>>::iterator begin() { return m_LayerStack.begin(); }
    std::vector<std::unique_ptr<Layer>>::iterator end() { return m_LayerStack.end(); }
    std::vector<std::unique_ptr<Layer>>::reverse_iterator rbegin() { return m_LayerStack.rbegin(); }
    std::vector<std::unique_ptr<Layer>>::reverse_iterator rend() { return m_LayerStack.rend(); }

    std::vector<std::unique_ptr<Layer>>::const_iterator begin() const { return m_LayerStack.begin(); }
    std::vector<std::unique_ptr<Layer>>::const_iterator end() const { return m_LayerStack.end(); }
    std::vector<std::unique_ptr<Layer>>::const_reverse_iterator rbegin() const { return m_LayerStack.rbegin(); }
    std::vector<std::unique_ptr<Layer>>::const_reverse_iterator rend() const { return m_LayerStack.rend(); }

    void SubmitCommand(LayerCommand command);
    void ProcessCommands();

#ifdef DEBUG
    // this is only used for the tests for now, so it will be taken out of the release build
    inline const std::vector<std::unique_ptr<Layer>> &GetLayers() const { return m_LayerStack; }
#endif
private:
    std::vector<LayerCommand> m_CommandQueue;
    std::vector<std::unique_ptr<Layer>> m_LayerStack;
    unsigned int m_LayerIndex = 0;
};
} // namespace SakuraVNE
