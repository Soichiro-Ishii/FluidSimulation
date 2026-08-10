#pragma once
#include "Application.h"
#include"StageManager.h"

class FluidSimApp final :
	public Application
{
private:
	StageManager m_stageManager;
protected:
	bool onInit() override;
	void onUpdate(float delta) override;
	void onRender() override;
	void onShutdown() override;
};

