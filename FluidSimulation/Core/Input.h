#pragma once
#include<GLFW/glfw3.h>
#include<glm/glm.hpp>
class Input
{
private:
	GLFWwindow* m_window = nullptr;
	bool m_keys[2][GLFW_KEY_LAST + 1];
	bool m_mouseButton[2][GLFW_MOUSE_BUTTON_LAST + 1];
	char m_now = 0;
	glm::vec2 m_mousePos[2];
	glm::vec2 m_mouseDelta = glm::vec2(0.0f, 0.0f);
	glm::vec2 m_wheelDelta = glm::vec2(0.0f, 0.0f);
	bool neverUpdated = true;
	bool firstFrame = false;
public:
	Input() = default;
	~Input() = default;

	void init(GLFWwindow* window);
	void update();
	bool isPress(int key) const;
	bool isTrigger(int key) const;
	bool isRelease(int key) const;

	bool isMousePress(int button) const;
	bool isMouseTrigger(int button) const;
	bool isMouseRelease(int button) const;
	glm::vec2 mousePos() const;
	glm::vec2 mouseDelta() const;
	glm::vec2 wheelDelta() const;

	static void scrollCallback(
		GLFWwindow* window,
		double xOffset,
		double yOffset);
};

