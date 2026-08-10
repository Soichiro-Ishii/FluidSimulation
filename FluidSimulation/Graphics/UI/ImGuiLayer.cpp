#include "pch.h"
#include "ImGuiLayer.h"
using namespace ImGui;

ImGuiLayer::ImGuiLayer(GLFWwindow* window) {
	init(window);
}

ImGuiLayer::~ImGuiLayer() {
	release();
}

bool ImGuiLayer::init(GLFWwindow* window) {
	IMGUI_CHECKVERSION();
	CreateContext();
	ImGuiIO& _io = GetIO();
	_io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	StyleColorsDark();
	if (!ImGui_ImplGlfw_InitForOpenGL(window, true)) {
		spdlog::critical("Failed to initialize Dear ImGui GLFW platform backend for OpenGL.");
		ImGui::DestroyContext();
		return false;
	}
	if (!ImGui_ImplOpenGL3_Init("#version 460 core")) {
		spdlog::critical(
			"Failed to initialize Dear ImGui OpenGL3 renderer backend."
		);
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
		return false;
	}
	m_active = true;
	return true;
}
void ImGuiLayer::beginFrame() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	NewFrame();
}
void ImGuiLayer::endFrame() {
	EndFrame();
}
void ImGuiLayer::render() {
	Render();
	ImGui_ImplOpenGL3_RenderDrawData(GetDrawData());
}
void ImGuiLayer::release() {
	if (m_active) {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		DestroyContext();
		m_active = false;
	}
}