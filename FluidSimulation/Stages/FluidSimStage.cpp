#include "pch.h"
#include "FluidSimStage.h"
#include"ProcMeshGenerator.h"
FluidSimStage::FluidSimStage()
{
	setStageName();
}
bool FluidSimStage::onInit() {

	m_lastShader.load("assets\\shaders\\screenVS.glsl", "assets\\shaders\\renderTexFS.glsl");
	if (!m_lastShader.valid()) {
		spdlog::critical("faild to load lastShader");
		return false;
	}
	m_initFluidShader.load("assets\\shaders\\screenVS.glsl", "assets\\shaders\\initFluidFS.glsl");
	if (!m_initFluidShader.valid()) {
		spdlog::critical("faild to load lastShader");
		return false;
	}
	m_advShader.load("assets\\shaders\\screenVS.glsl", "assets\\shaders\\advectionFS.glsl");
	if (!m_advShader.valid()) {
		spdlog::critical("faild to load advShader");
		return false;
	}
	TEXTURE2DSETTING firstColorDensSet;
	firstColorDensSet.filter = TEXTURE2DFILTER::LINEAR;
	firstColorDensSet.wrap = TEXTURE2DWRAP::CLAMP_TO_EDGE;
	std::string path = "assets\\data\\textures\\obama.jpg";
	m_firstColDens.load(path, firstColorDensSet);
	GLMeshData meshData = ProcMeshGenerator::createScreen();
	m_screen.create(meshData);
	TEXTURE2DSETTING texSet;
	texSet.filter = TEXTURE2DFILTER::LINEAR;
	texSet.wrap = TEXTURE2DWRAP::CLAMP_TO_EDGE;
	std::vector<TEXTURE2DDESC> texDescs(2);
	texDescs[0].width = width();
	texDescs[0].height = height();
	texDescs[0].internalFormat = GL_RG16F;
	texDescs[0].format = GL_RG;
	texDescs[0].type = GL_FLOAT;
	texDescs[0].set = texSet;
	texDescs[1] = texDescs[0];	//共通部分はコピー
	texDescs[1].internalFormat = GL_RGBA16F;
	texDescs[1].format = GL_RGBA;
	for (auto& rt : m_rt)
		rt.create(texDescs);


	m_FSC.resolution.x = widthf();
	m_FSC.resolution.y = heightf();
	m_FSC.time = 0.0f;
	m_FSC.delta = 0.0f;
	m_FSCUB.create(&m_FSC, sizeof(FluidSimConstants), 0);

	glDisable(GL_DEPTH_TEST);
	return true;
}
void FluidSimStage::onUpdate(float delta) {
	m_currentRT ^= 1;
	m_FSC.time += delta;
	m_FSC.delta = delta;
	m_FSCUB.update(&m_FSC, sizeof(FluidSimConstants), 0);
}
void FluidSimStage::onRender() {
	if (firstFrame) {
		for (auto& rt : m_rt) {
			rt.bind();
			m_initFluidShader.bind();
			m_firstColDens.bind(0);
			m_screen.draw();
			rt.unbind();
		}
		firstFrame = false;
	}

	glViewport(0, 0, width(), height());
	m_rt[m_currentRT].bind();
	m_advShader.bind();
	m_rt[m_currentRT ^ 1].color(0).bind(0);
	m_rt[m_currentRT ^ 1].color(1).bind(1);
	m_screen.draw();
	m_rt[m_currentRT].unbind();

	glViewport(0, 0, width(), height());
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	m_lastShader.bind();
	m_rt[m_currentRT].color(1).bind(0);
	m_screen.draw();
}
void FluidSimStage::onShutdown() {}

void FluidSimStage::setStageName()
{
	m_name = "FluidSimStage";
}