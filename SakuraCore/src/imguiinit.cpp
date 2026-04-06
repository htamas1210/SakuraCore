#include "imguiinit.h"
#include "Application.h"
#include "Log.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

namespace SakuraVNE {
ImGuiInit::ImGuiInit() { Init(); }
ImGuiInit::~ImGuiInit() { Shutdown(); }

void ImGuiInit::Init() {
    // Imgui init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForSDLRenderer(Application::Get().GetSDLWindow(), Application::Get().GetSDLRenderer());
    ImGui_ImplSDLRenderer3_Init(Application::Get().GetSDLRenderer());

    LOG_INFO("Imgui layer OnAttach ran");
}

void ImGuiInit::Shutdown() {
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    LOG_WARN("Imgui on detach ran");
}

void ImGuiInit::Begin() {
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
    // for docking the imgui windows to the edge of the sdl window
    ImGui::DockSpaceOverViewport();
}

void ImGuiInit::End() {
    ImGuiIO &io = ImGui::GetIO();

    Application &app = Application::Get();
    io.DisplaySize = ImVec2((float)app.GetAppData().windowdata.width, (float)app.GetAppData().windowdata.height);

    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), Application::Get().GetSDLRenderer());
    ImGui::EndFrame();
}
} // namespace SakuraVNE
