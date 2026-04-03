#pragma once

namespace SakuraVNE {
class ImGuiInit {
public:
    ImGuiInit();
    ~ImGuiInit();

    void Init();
    void Shutdown();
    void Begin();
    void End();
};
} // namespace SakuraVNE
