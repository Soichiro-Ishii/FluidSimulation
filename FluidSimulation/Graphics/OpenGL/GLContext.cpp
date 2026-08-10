#include "pch.h"
#include "GLContext.h"

GLContext::~GLContext() {
	release();
}
GLContext::GLContext(GLContext&& other) noexcept {
	m_window = std::exchange(other.m_window, nullptr);
	m_width = std::exchange(other.m_width, 0);
	m_height = std::exchange(other.m_height, 0);
	m_fullScreen = std::exchange(other.m_fullScreen, false);
	m_VSync = std::exchange(other.m_VSync, 1);
}
GLContext& GLContext::operator=(GLContext&& other) noexcept {
	if (this != &other) {
		release();
		m_window = std::exchange(other.m_window, nullptr);
		m_width = std::exchange(other.m_width, 0);
		m_height = std::exchange(other.m_height, 0);
		m_fullScreen = std::exchange(other.m_fullScreen, false);
		m_VSync = std::exchange(other.m_VSync, 1);
	}
	return *this;
}
int GLContext::create(int width, int height, std::string windowName, bool fullScreen, int VSync) {
	m_width = width;
	m_height = height;
	m_fullScreen = fullScreen;
	m_VSync = VSync;
	//glfw初期化
	if (!glfwInit()) {
		spdlog::critical("faild initialization glfw");
		return -1;
	}
	spdlog::info("glfw initialized");
	//バージョン指定
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//coreにする
	glfwWindowHint(GLFW_OPENGL_PROFILE,
		GLFW_OPENGL_CORE_PROFILE);
	//デバッグ出力有効
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
	//sRGB
	glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE);
	//ウィンドウ作成
	if (fullScreen) {
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();
		const GLFWvidmode* mode = glfwGetVideoMode(monitor);

		glfwWindowHint(GLFW_RED_BITS, mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
		m_width = mode->width;
		m_height = mode->height;
		m_window = glfwCreateWindow(m_width, m_height, windowName.c_str(), monitor, nullptr);
	}
	else {
		m_window = glfwCreateWindow(m_width, m_height, windowName.c_str(), nullptr, nullptr);
	}
	if (!m_window) {
		spdlog::critical("faild creating window");
		glfwTerminate();
		spdlog::info("terminated glfw");
		return -1;
	}
	spdlog::info("window created");
	//コンテキスト作成
	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(VSync);		//VSync
	//gladのglロード
	if (!gladLoadGL(glfwGetProcAddress)) {
		spdlog::critical("faild loading gl");
		release();
		return -1;
	}
	spdlog::info("gl loded");

	//デバッグ出力有効
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

	glDebugMessageCallback(
		OpenGLDebugCallback,
		nullptr
	);
	glfwGetFramebufferSize(m_window, &m_width, &m_height);
	return 0;
}
void GLContext::update() {
	glfwGetFramebufferSize(m_window, &m_width, &m_height);
}
void GLContext::release() {
	if (m_window != nullptr) {
		GLFWwindow* window = std::exchange(m_window, nullptr);
		glfwDestroyWindow(window);
		spdlog::info("destroied window");
		glfwTerminate();
		spdlog::info("terminated glfw");
	}
	m_width = 0;
	m_height = 0;
}

static void GLAPIENTRY OpenGLDebugCallback(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam)
{
	switch (severity)
	{
	case GL_DEBUG_SEVERITY_HIGH:
		spdlog::critical(
			"OpenGL HIGH error [{}]: {}",
			id,
			message
		);
		break;

	case GL_DEBUG_SEVERITY_MEDIUM:
		spdlog::error(
			"OpenGL MEDIUM error [{}]: {}",
			id,
			message
		);
		break;

	case GL_DEBUG_SEVERITY_LOW:
		spdlog::warn(
			"OpenGL LOW error [{}]: {}",
			id,
			message
		);
		break;

	case GL_DEBUG_SEVERITY_NOTIFICATION:
		spdlog::debug(
			"OpenGL notification [{}]: {}",
			id,
			message
		);
		break;
	}
}