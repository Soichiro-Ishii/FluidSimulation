#pragma once
#include"GLContext.h"
#include"FPSLimiter.h"
#include"Input.h"
#include"ImGuiLayer.h"
class Application
{
private:
	GLContext m_context;
	CHRONO m_chrono;
	FPSLimiter m_FPSLimiter;
	Input m_input;
	ImGuiLayer m_imguilayer;
	bool m_quit = false;
public:
	Application() = default;
	virtual ~Application() = default;

	Application(Application&) = delete;
	Application& operator=(Application&) = delete;

	int run(int width, int height, std::string windowName, bool fullScreen = false, int VSync = 1, float targetFPS = 0.0f);
protected:
	virtual bool onInit() = 0;
	virtual void onUpdate(float delta) = 0;
	virtual void onRender() = 0;
	virtual void onShutdown() = 0;
	void quit();
public:
	[[nodiscard]] GLFWwindow* window() const {
		return m_context.window();
	}
	[[nodiscard]] int width() const {
		return m_context.width();
	}
	[[nodiscard]] int height() const {
		return m_context.height();
	}
	[[nodiscard]] float widthf() const {
		return m_context.widthf();
	}
	[[nodiscard]] float heightf() const {
		return m_context.heightf();
	}
	[[nodiscard]] const Input& input() const {
		return m_input;
	}
	[[nodiscard]] const bool isPress(int key) const {
		if (!ImGui::GetIO().WantCaptureKeyboard)
			return m_input.isPress(key);
		else
			return false;
	}
	[[nodiscard]] const bool isTrigger(int key) const {
		if (!ImGui::GetIO().WantCaptureKeyboard)
			return m_input.isTrigger(key);
		else
			return false;
	}
	void setTargetFPS(float newFPS) {
		m_FPSLimiter.setTargetFPS(static_cast<double>(newFPS));
	}
};

