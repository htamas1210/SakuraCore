#include "Layer.h"
#include "Application.h"
#include <memory>
#include <string>

namespace SakuraVNE {
Layer::Layer(const std::string &name, bool isActive) : m_LayerName(name), m_isActive(isActive) {}

void Layer::QueueTransition(std::unique_ptr<Layer> toLayer) {
    // TODO: redo this based on the event video
    auto &layerStack = Application::Get().m_LayerStack;
    for (auto &layer : layerStack) {
        if (layer.get() == this) {
            layer = std::move(toLayer);
            return;
        }
    }
}
} // namespace SakuraVNE
