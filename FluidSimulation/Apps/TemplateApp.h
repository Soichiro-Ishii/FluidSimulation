#pragma once
#include "Application.h"
class TemplateApp final :
	public Application
{
protected:
	bool onInit() override;
	void onUpdate(float delta) override;
	void onRender() override;
	void onShutdown() override;
};

