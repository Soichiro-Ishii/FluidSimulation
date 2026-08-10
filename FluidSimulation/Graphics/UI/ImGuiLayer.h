#pragma once
#include <glad/gl.h>
#include<GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
class ImGuiLayer
{
private:
	bool m_active = false;
public:
	ImGuiLayer() = default;
	ImGuiLayer(GLFWwindow* window);
	~ImGuiLayer();
	bool init(GLFWwindow* window);
	void beginFrame();
	void endFrame();
	void render();
	void release();
};

