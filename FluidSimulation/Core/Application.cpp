#include "pch.h"
#include "Application.h"
int Application::run(int _width, int _height, std::string windowName, bool fullScreen, int VSync, float targetFPS) {
	//垂直同期ありのときはそっちを優先
	if (VSync) m_FPSLimiter.setTargetFPS(static_cast<double>(0.0f));
	else m_FPSLimiter.setTargetFPS(static_cast<double>(targetFPS));
	if (m_context.create(_width, _height, windowName.c_str(), fullScreen, VSync) != 0) {
		spdlog::critical("faild creating context");
		return -1;
	}
	if (!m_imguilayer.init(window())) {
		spdlog::critical("Faild to init ImGui.");
		return -1;
	}
	if (!onInit()) {
		spdlog::critical("faild application initialization");
		onShutdown();
		return -1;
	}
	//sRGB
	glEnable(GL_FRAMEBUFFER_SRGB);
	//delta平均取る用配列
	std::vector<float> deltas;
	size_t deltaIdx = 0;
	size_t maxNumDelta = 0;
	if (VSync) {
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		maxNumDelta = mode->refreshRate;
	}
	else {
		maxNumDelta = static_cast<size_t>(targetFPS);
		if (maxNumDelta <= 0) maxNumDelta = 60;
	}
	deltas.resize(maxNumDelta);
	m_chrono.timeStart();
	m_input.init(m_context.window());
	while (!glfwWindowShouldClose(m_context.window()) && !m_quit) {
		m_chrono.timeEnd();
		float delta = static_cast<float>(m_chrono.getElapsed());
		//最小化モードのときは記録しない
		if (!(width() <= 0 || height() <= 0)) {
			deltas[deltaIdx] = delta;
			deltaIdx++;
		}
		m_chrono.timeStart();
		m_FPSLimiter.begin();

		//イベント処理
		glfwPollEvents();

		m_context.update();
		m_input.update();
		m_imguilayer.beginFrame();

		onUpdate(delta);
		//最小化モードのときは描画しない
		if (width() <= 0 || height() <= 0) {
			m_imguilayer.endFrame();
			glfwWaitEvents();
			continue;
		}
		onRender();

		m_imguilayer.render();

		//画面スワップ
		glfwSwapBuffers(m_context.window());
		m_FPSLimiter.wait();
		if (deltaIdx >= maxNumDelta) {
			float sumDelta = 0.0f;
			for (auto d : deltas) {
				sumDelta += d;
			}
			float avgDelta = sumDelta / maxNumDelta;
			spdlog::info("delta:{}\nfps:{}", avgDelta, 1 / avgDelta);
			deltaIdx = 0;
		}
	}
	onShutdown();
	m_imguilayer.release();
	return 0;
}

void Application::quit() {
	m_quit = true;
}