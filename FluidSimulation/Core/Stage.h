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
	[[nodiscard]] const bool isPress(int key) const {
		return m_app->isPress(key);
	}
	[[nodiscard]] const bool isTrigger(int key) const {
		return m_app->isTrigger(key);
	}
protected:
	std::string m_name = "default";
	const Application* m_app = nullptr;
};

