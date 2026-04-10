#include "Layer.h"
#include "Application.h"
#include "LayerStack.h"
#include <memory>
#include <string>

namespace SakuraVNE {
Layer::Layer(const std::string &name, bool isActive) : m_LayerName(name), m_isActive(isActive) {}

void Layer::QueueTransition(std::unique_ptr<Layer> toLayer) {
    auto &layerStack = Application::Get().GetLayerStack();

    LayerCommand command;
    command.action = LayerAction::Transition;
    command.targetLayer = this;
    command.newLayer = std::move(toLayer);

    layerStack.SubmitCommand(std::move(command));
}
} // namespace SakuraVNE
