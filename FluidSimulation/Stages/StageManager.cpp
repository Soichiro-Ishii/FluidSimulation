#include "pch.h"
#include "StageManager.h"

void StageManager::setStageManager(const Application* app)
{
	m_app = app;
}

bool StageManager::change(std::unique_ptr<Stage> stage)
{
	if (m_app == nullptr) {
		spdlog::critical("stage manager is not set application.");
		return false;
	}
	if (m_active && m_stage->name() == stage->name())
		return true;
	if (m_active) {
		onShutdown();
	}
	m_stage = std::move(stage);
	if (!onInit()) {
		spdlog::info("faild to init stage : {}", m_stage->name());
		return false;
	}
	spdlog::info("active stage : {}", m_stage->name());
	m_active = true;
	return true;
}

bool StageManager::onInit()
{
	if (m_app == nullptr) {
		spdlog::critical("stage manager is not set application.");
		return false;
	}
	m_stage->setApp(m_app);
	return m_stage->onInit();
}

void StageManager::onUpdate(float delta)
{
	if (!m_active)
		return;
	m_stage->onUpdate(delta);
}

void StageManager::onRender()
{
	if (!m_active)
		return;
	m_stage->onRender();
}

void StageManager::onShutdown()
{
	if (!m_active)
		return;
	std::string stageName = m_stage->name();
	m_stage->onShutdown();
	spdlog::info("shutdown stage : {}", stageName);
	m_active = false;
}
