#include "Layer.h"
#include "Application.h"
#include "LayerStack.h"
#include <memory>
#include <string>

namespace SakuraVNE {
Layer::Layer(const std::string &name, bool isActive) : m_LayerName(name), m_isActive(isActive) {}

void Layer::QueueTransition(std::unique_ptr<Layer> toLayer) {
    auto &layerStack = Application::Get().GetLayerStack();
    // TODO: redo this based on the event video
    // basically handle it like the other layer stuff
    // put it into a vector and go over the list at the end of the application loop
    auto &layerStack = Application::Get().m_LayerStack;

    LayerCommand command;
    command.action = LayerAction::Transition;
    command.targetLayer = this;
    command.newLayer = std::move(toLayer);

    layerStack.SubmitCommand(std::move(command));

    // for (auto &layer : layerStack) {
    //     if (layer.get() == this) {
    //         layer = std::move(toLayer);
    //         return;
    //     }
    // }
}
} // namespace SakuraVNE
