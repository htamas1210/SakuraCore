#include "imguilayer.h"
#include "Application.h"
#include "Log.h"
#include "imgui.h"
#include "imgui_impl_sdl3.h"
#include "imgui_impl_sdlrenderer3.h"

namespace SakuraVNE {
ImGuiLayer::ImGuiLayer() : Layer("ImGuiLayer") {}

void ImGuiLayer::OnAttach() {
    // Imgui init
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    ImGui::StyleColorsDark();

    ImGui_ImplSDL3_InitForSDLRenderer(Application::Get().GetSDLWindow(), Application::Get().GetSDLRenderer());
    ImGui_ImplSDLRenderer3_Init(Application::Get().GetSDLRenderer());

    LOG_INFO("Imgui layer OnAttach ran");
}

void ImGuiLayer::OnDetach() {
    ImGui_ImplSDLRenderer3_Shutdown();
    ImGui_ImplSDL3_Shutdown();
    ImGui::DestroyContext();
    LOG_WARN("Imgui on detach ran");
}

void ImGuiLayer::Begin() {
    ImGui_ImplSDLRenderer3_NewFrame();
    ImGui_ImplSDL3_NewFrame();
    ImGui::NewFrame();
}

void ImGuiLayer::End() {
    ImGuiIO &io = ImGui::GetIO();

    Application &app = Application::Get();
    io.DisplaySize = ImVec2((float)app.GetWindowData().width, (float)app.GetWindowData().height);

    ImGui::Render();
    ImGui_ImplSDLRenderer3_RenderDrawData(ImGui::GetDrawData(), Application::Get().GetSDLRenderer());
    ImGui::EndFrame();
}

void ImGuiLayer::OnImGuiRender() {
    ImGuiIO &io = ImGui::GetIO();
    bool demoWindow = true;
    ImGui::Begin("Test window");
    ImGui::ShowDemoWindow(&demoWindow);
    ImGui::Text("Application avg %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

    if (ImGui::Button("Quit")) {
        Application::Get().SetRunningState(false);
    }
    ImGui::End();
}
} // namespace SakuraVNE
