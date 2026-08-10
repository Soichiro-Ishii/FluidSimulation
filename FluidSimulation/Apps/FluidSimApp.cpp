#include "pch.h"
#include "FluidSimApp.h"
#include"ProcMeshGenerator.h"
#include"FluidSimStage.h"

bool FluidSimApp::onInit() {
	m_stageManager.setStageManager(this);
	return m_stageManager.change(std::make_unique<FluidSimStage>());
}
void FluidSimApp::onUpdate(float delta) {
	bool stageChangeResult = true;
	if (isTrigger(GLFW_KEY_F1))
		stageChangeResult = m_stageManager.change(std::make_unique<FluidSimStage>());
	if (!stageChangeResult)
		quit();

	m_stageManager.onUpdate(delta);
}
void FluidSimApp::onRender() {
	m_stageManager.onRender();
}
void FluidSimApp::onShutdown() {
	spdlog::info("Application shutdown");
}