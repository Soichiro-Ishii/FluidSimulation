#pragma once
#include<GLFW/glfw3.h>
class Input
{
private:
	GLFWwindow* m_window = nullptr;
	bool m_keys[2][GLFW_KEY_LAST + 1];
	char m_now = 0;
public:
	Input() = default;
	~Input() = default;

	void init(GLFWwindow* window);
	void update();
	bool isPress(int key) const;
	bool isTrigger(int key) const;
};

