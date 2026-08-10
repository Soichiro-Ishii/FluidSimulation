#include "pch.h"
#include "TmpStage.h"
TmpStage::TmpStage()
{
	setStageName();
}
bool TmpStage::onInit() {
	glDisable(GL_DEPTH_TEST);
	return true;
}
void TmpStage::onUpdate(float delta) {}
void TmpStage::onRender() {
	glViewport(0, 0, width(), height());
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
void TmpStage::onShutdown() {}
void TmpStage::setStageName()
{
	m_name = "tmp";
}