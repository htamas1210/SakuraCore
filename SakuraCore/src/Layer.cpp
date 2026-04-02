#include "Layer.h"
#include <string>

namespace SakuraVNE {
Layer::Layer(const std::string &name, bool isActive) : m_LayerName(name), m_isActive(isActive) {}
} // namespace SakuraVNE
