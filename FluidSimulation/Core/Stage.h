#pragma once
#include<string>
#include"Application.h"
class Stage
{
public:
	Stage() = default;
	virtual ~Stage() = default;
	void setApp(const Application* app);
	virtual bool onInit() = 0;
	virtual void onUpdate(float delta) = 0;
	virtual void onRender() = 0;
	virtual void onShutdown() = 0;
	[[nodiscard]] std::string name() const {
		return m_name;
	}
protected:
	virtual void setStageName() = 0;

	[[nodiscard]] GLFWwindow* window() const {
		return m_app->window();
	}
	[[nodiscard]] int width() const {
		return m_app->width();
	}
	[[nodiscard]] int height() const {
		return m_app->height();
	}
	[[nodiscard]] float widthf() const {
		return m_app->widthf();
	}
	[[nodiscard]] float heightf() const {
		return m_app->heightf();
	}
	[[nodiscard]] const Input& input() const {
		return m_app->input();
	}
	[[nodiscard]] bool isPress(int key) const {
		return m_app->isPress(key);
	}
	[[nodiscard]] bool isTrigger(int key) const {
		return m_app->isTrigger(key);
	}
	[[nodiscard]] bool isRelease(int key) const {
		return m_app->isRelease(key);
	}
	[[nodiscard]] bool isMousePress(int button) const {
		return m_app->isMousePress(button);
	}
	[[nodiscard]] bool isMouseTrigger(int button) const {
		return m_app->isMouseTrigger(button);
	}
	[[nodiscard]] bool isMouseRelease(int button) const {
		return m_app->isMouseRelease(button);
	}
	[[nodiscard]] std::optional<glm::vec2> mousePos() const {
		return m_app->mousePos();
	}
	[[nodiscard]] glm::vec2 mouseDelta() const {
		return m_app->mouseDelta();
	}
	[[nodiscard]] glm::vec2 wheelDelta() const {
		return m_app->wheelDelta();
	}
protected:
	std::string m_name = "default";
	const Application* m_app = nullptr;
};

