#pragma once
#include<GLFW/glfw3.h>
#include<glad/gl.h>
#include<string>

class GLContext
{
private:
	GLFWwindow* m_window = nullptr;
	int m_width = 0;
	int m_height = 0;
	bool m_fullScreen = false;
	int m_VSync = 1;
public:
	GLContext() = default;
	~GLContext();
	GLContext(GLContext&) = delete;
	GLContext& operator=(GLContext&) = delete;
	GLContext(GLContext&& other) noexcept;
	GLContext& operator=(GLContext&& other) noexcept;
	int create(int width, int height, std::string windowName, bool fullScreen = false, int VSync = 1);
	void update();
	void release();
	[[nodiscard]] GLFWwindow* window() const {
		return m_window;
	}
	[[nodiscard]] int width() const {
		return m_width;
	}
	[[nodiscard]] int height() const {
		return m_height;
	}
	[[nodiscard]] float widthf() const {
		return static_cast<float>(m_width);
	}
	[[nodiscard]] float heightf() const {
		return static_cast<float>(m_height);
	}
};

static void GLAPIENTRY OpenGLDebugCallback(
	GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam);

