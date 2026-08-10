#include "pch.h"
#include "TemplateApp.h"

bool TemplateApp::onInit() {
	glClearDepth(1.0f);
	return true;
}
void TemplateApp::onUpdate(float delta) {

}
void TemplateApp::onRender() {
	//画面クリア色設定&深度クリアの値設定
	glClearColor(0.2f, 0.8f, 1.0f, 0.0f);
	//画面クリア
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void TemplateApp::onShutdown() {

}