#include "Layer.h"
#include <string>

namespace SakuraVNE {
Layer::Layer(const std::string &name) : m_LayerName(name), isActive(true) {}
} // namespace SakuraVNE
