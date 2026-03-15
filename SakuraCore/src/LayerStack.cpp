#include "LayerStack.h"
#include <algorithm>
#include <vector>

namespace SakuraVNE {
LayerStack::~LayerStack() {
    for (Layer *layer : m_LayerStack) {
        layer->OnDetach();
        delete layer;
    }
}

void LayerStack::PushLayer(Layer *layer) {
    m_LayerStack.emplace(m_LayerStack.begin() + m_LayerIndex, layer);
    m_LayerIndex++;
}

void LayerStack::PopLayer(Layer *layer) {
    auto match = std::find(m_LayerStack.begin(), m_LayerStack.begin() + m_LayerIndex, layer);
    if (match != m_LayerStack.begin() + m_LayerIndex) {
        layer->OnDetach();
        m_LayerStack.erase(match);
        m_LayerIndex--;
    }
}

void LayerStack::PushOverLay(Layer *layer) { m_LayerStack.emplace_back(layer); }

void LayerStack::PopOverlay(Layer *layer) {
    auto match = std::find(m_LayerStack.begin() + m_LayerIndex, m_LayerStack.end(), layer);

    if (match != m_LayerStack.end()) {
        layer->OnDetach();
        m_LayerStack.erase(match);
    }
}
} // namespace SakuraVNE
