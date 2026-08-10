#include "pch.h"
#include "Input.h"
void Input::init(GLFWwindow* window) {
	m_window = window;
	for (int i = 0; i <= 1; i++) {
		for (int j = 0; j <= GLFW_KEY_LAST; j++) {
			m_keys[i][j] = false;
		}
	}
	for (auto& pos : m_mousePos)
		pos = glm::vec2(0.0, 0.0);
	glfwSetWindowUserPointer(m_window, this);

	glfwSetScrollCallback(
		m_window,
		Input::scrollCallback
	);
}
void Input::update() {
	m_now = m_now ^ 1;
	if (!m_window) {
		spdlog::error("Input:GLFWWindow is not set.");
		return;
	}
	//キー
	for (int i = 0; i <= GLFW_KEY_LAST; i++) {
		m_keys[m_now][i] = glfwGetKey(m_window, i);
	}
	//マウス
	double x, y;
	glfwGetCursorPos(m_window, &x, &y);
	m_mousePos[m_now] = glm::vec2(x, y);
	for (int i = 0; i <= GLFW_MOUSE_BUTTON_LAST; i++) {
		m_mouseButton[m_now][i] = glfwGetMouseButton(m_window, i);
	}

	if (firstFrame)
		firstFrame = false;
	if (neverUpdated) {
		neverUpdated = false;
		firstFrame = true;
	}
}
bool Input::isPress(int key) const {
	if (!m_window) {
		spdlog::error("Input:GLFWWindow is not set.");
		return false;
	}
	if (key < 0 || key > GLFW_KEY_LAST) {
		spdlog::error("Invalid key: {}", key);
		return false;
	}
	return m_keys[m_now][key];
}
bool Input::isTrigger(int key) const {
	if (!m_window) {
		spdlog::error("Input:GLFWWindow is not set.");
		return false;
	}
	if (key < 0 || key > GLFW_KEY_LAST) {
		spdlog::error("Invalid key: {}", key);
		return false;
	}
	char before = m_now ^ 1;
	//前のキーは押されていなくて、今は押されている
	return m_keys[m_now][key] && !m_keys[before][key];
}

bool Input::isRelease(int key) const {
	if (!m_window) {
		spdlog::error("Input:GLFWWindow is not set.");
		return false;
	}
	if (key < 0 || key > GLFW_KEY_LAST) {
		spdlog::error("Invalid key: {}", key);
		return false;
	}
	char before = m_now ^ 1;
	//前のキーは押されていて、今は押されていない
	return !m_keys[m_now][key] && m_keys[before][key];
}

bool Input::isMousePress(int button) const {
	if (!m_window) {
		spdlog::error("Input:GLFWWindow is not set.");
		return false;
	}
	if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST) {
		spdlog::error("Invalid mouse button: {}", button);
		return false;
	}
	return m_mouseButton[m_now][button];
}
bool Input::isMouseTrigger(int button) const {
	if (!m_window) {
		spdlog::error("Input:GLFWWindow is not set.");
		return false;
	}
	if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST) {
		spdlog::error("Invalid mouse button: {}", button);
		return false;
	}
	char before = m_now ^ 1;
	return m_mouseButton[m_now][button] && !m_mouseButton[before][button];
}
bool Input::isMouseRelease(int button) const {
	if (!m_window) {
		spdlog::error("Input:GLFWWindow is not set.");
		return false;
	}
	if (button < 0 || button > GLFW_MOUSE_BUTTON_LAST) {
		spdlog::error("Invalid mouse button: {}", button);
		return false;
	}
	char before = m_now ^ 1;
	return !m_mouseButton[m_now][button] && m_mouseButton[before][button];
}
glm::vec2 Input::mousePos() const {
	return m_mousePos[m_now];
}
glm::vec2 Input::mouseDelta() const {
	//最初のフレームは速度0
	if (firstFrame)
		return glm::vec2(0.0f, 0.0f);
	return m_mousePos[m_now] - m_mousePos[m_now ^ 1];
}
glm::vec2 Input::wheelDelta() const {
	return m_wheelDelta;
}

void Input::scrollCallback(
	GLFWwindow* window,
	double xOffset,
	double yOffset)
{
	auto* input = static_cast<Input*>(glfwGetWindowUserPointer(window));

	input->m_wheelDelta.x +=
		static_cast<float>(xOffset);

	input->m_wheelDelta.y +=
		static_cast<float>(yOffset);
}