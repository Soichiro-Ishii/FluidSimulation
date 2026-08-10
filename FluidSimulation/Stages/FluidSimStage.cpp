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
	m_initColDensShader.load("assets\\shaders\\screenVS.glsl", "assets\\shaders\\initColDensFS.glsl");
	if (!m_initColDensShader.valid()) {
		spdlog::critical("faild to load init color and density Shader");
		return false;
	}
	m_initVelShader.load("assets\\shaders\\screenVS.glsl", "assets\\shaders\\initVelFS.glsl");
	if (!m_initVelShader.valid()) {
		spdlog::critical("faild to load init velocity Shader");
		return false;
	}
	m_initPressureShader.load("assets\\shaders\\screenVS.glsl", "assets\\shaders\\initPressureFS.glsl");
	if (!m_initPressureShader.valid()) {
		spdlog::critical("faild to load init pressure Shader");
		return false;
	}
	m_advVelShader.load("assets\\shaders\\screenVS.glsl", "assets\\shaders\\advectionVelFS.glsl");
	if (!m_advVelShader.valid()) {
		spdlog::critical("faild to load advection velocity Shader");
		return false;
	}
	m_advColDensShader.load("assets\\shaders\\screenVS.glsl", "assets\\shaders\\advectionColDensFS.glsl");
	if (!m_advColDensShader.valid()) {
		spdlog::critical("faild to load advection color and density Shader");
		return false;
	}
	m_divergenceShader.load("assets\\shaders\\screenVS.glsl", "assets\\shaders\\divergenceFS.glsl");
	if (!m_divergenceShader.valid()) {
		spdlog::critical("faild to load divergence Shader");
		return false;
	}
	m_renderDivShader.load("assets\\shaders\\screenVS.glsl", "assets\\shaders\\renderDivergenceFS.glsl");
	if (!m_divergenceShader.valid()) {
		spdlog::critical("faild to load render divergence Shader");
		return false;
	}
	m_jacobiPressureShader.load("assets\\shaders\\screenVS.glsl", "assets\\shaders\\jacobiPressureFS.glsl");
	if (!m_divergenceShader.valid()) {
		spdlog::critical("faild to load jacobi pressure Shader");
		return false;
	}
	m_projectionShader.load("assets\\shaders\\screenVS.glsl", "assets\\shaders\\projectionFS.glsl");
	if (!m_divergenceShader.valid()) {
		spdlog::critical("faild to load projection Shader");
		return false;
	}
	m_applyForceVelShader.load("assets\\shaders\\screenVS.glsl", "assets\\shaders\\applyForceVelFS.glsl");
	if (!m_divergenceShader.valid()) {
		spdlog::critical("faild to load apply force to velocity Shader");
		return false;
	}

	TEXTURE2DSETTING firstColorDensSet;
	firstColorDensSet.filter = TEXTURE2DFILTER::LINEAR;
	firstColorDensSet.wrap = TEXTURE2DWRAP::CLAMP_TO_EDGE;
	firstColorDensSet.colorSpace = COLOR_SPACE::SRGB;
	std::string path = "assets\\data\\textures\\fractal.png";

	m_firstColDens.load(path, firstColorDensSet);
	GLMeshData meshData = ProcMeshGenerator::createScreen();
	m_screen.create(meshData);

	TEXTURE2DSETTING texSet;
	texSet.filter = TEXTURE2DFILTER::LINEAR;
	texSet.wrap = TEXTURE2DWRAP::REPEAT;
	TEXTURE2DDESC velDesc;
	TEXTURE2DDESC colDensDesc;
	TEXTURE2DDESC divergenceDesc;
	TEXTURE2DDESC pressureDesc;
	velDesc.width = width();
	velDesc.height = height();
	velDesc.internalFormat = GL_RG16F;
	velDesc.format = GL_RG;
	velDesc.type = GL_FLOAT;
	velDesc.set = texSet;
	colDensDesc = velDesc;	//共通部分はコピー
	colDensDesc.internalFormat = GL_RGBA16F;
	colDensDesc.format = GL_RGBA;
	divergenceDesc = velDesc;	//共通部分はコピー
	divergenceDesc.internalFormat = GL_R16F;
	divergenceDesc.format = GL_RED;
	divergenceDesc.set.filter = TEXTURE2DFILTER::NEAREST;
	pressureDesc = divergenceDesc;		//共通部分はコピー
	for (auto& rt : m_velRT)
		rt.create(velDesc);
	for (auto& rt : m_colDensRT)
		rt.create(colDensDesc);
	m_divergenceRT.create(divergenceDesc);
	for (auto& rt : m_pressureRT)
		rt.create(pressureDesc);

	SAMPLER_DESC smpDesc;
	smpDesc.minFilter = TEXTURE2DFILTER::NEAREST;
	smpDesc.magFilter = TEXTURE2DFILTER::NEAREST;
	smpDesc.wrapS = TEXTURE2DWRAP::REPEAT;
	smpDesc.wrapT = TEXTURE2DWRAP::REPEAT;
	smpDesc.wrapR = TEXTURE2DWRAP::REPEAT;
	m_texcelSMP.create(smpDesc);

	m_FSC.resolution.x = widthf();
	m_FSC.resolution.y = heightf();
	m_FSC.time = 0.0f;
	m_FSC.delta = 0.0f;
	m_FSCUB.create(&m_FSC, sizeof(FluidSimConstants), 0);

	m_MS.mPos = glm::vec2(0.0f, 0.0f);
	m_MS.mVel = glm::vec2(0.0f, 0.0f);
	m_MS.interactionForce = 10000;
	m_MS.interactionRadius = 0.025;
	m_MS.gravity = glm::vec2(0.0f, 0.0f);
	m_MS.otherForce = glm::vec2(0.0f, 0.0f);
	m_MS.otherAcc = glm::vec2(0.0f, 0.0f);
	m_MS.injectColor = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
	m_MS.rClick = 0;
	m_MS.lClick = 0;
	m_MSUB.create(&m_MS, sizeof(FluidSimInput), 1);

	glDisable(GL_DEPTH_TEST);
	return true;
}
void FluidSimStage::onUpdate(float delta) {
	m_currentRT ^= 1;
	m_FSC.time += delta;
	m_FSC.delta = delta;
	m_FSCUB.update(&m_FSC, sizeof(FluidSimConstants), 0);
	auto mPos = mousePos();
	if (mPos != std::nullopt)
		m_MS.mPos = *mPos;
	m_MS.mVel = mouseDelta() / delta;
	m_MS.rClick = static_cast<uint32_t>(isMousePress(GLFW_MOUSE_BUTTON_RIGHT));
	m_MS.lClick = static_cast<uint32_t>(isMousePress(GLFW_MOUSE_BUTTON_LEFT));
	m_MSUB.update(&m_MS, sizeof(FluidSimInput), 0);
}
void FluidSimStage::onRender() {
	glViewport(0, 0, m_velRT[m_currentRT].width(), m_velRT[m_currentRT].height());
	if (firstFrame) {
		for (auto& rt : m_velRT) {
			rt.bind();
			m_initVelShader.bind();
			m_screen.draw();
			rt.unbind();
		}
		for (auto& rt : m_colDensRT) {
			rt.bind();
			m_initColDensShader.bind();
			m_firstColDens.bind(0);
			m_screen.draw();
			rt.unbind();
		}
		firstFrame = false;
	}
	for (auto& rt : m_pressureRT) {
		rt.bind();
		m_initPressureShader.bind();
		m_screen.draw();
		rt.unbind();
	}
	//速度移流
	m_currentVelRT ^= 1;
	m_velRT[m_currentVelRT].bind();
	m_advVelShader.bind();
	m_velRT[m_currentVelRT ^ 1].color().bind(0);
	m_screen.draw();
	m_velRT[m_currentVelRT].unbind();
	//外力与える
	m_currentVelRT ^= 1;
	m_velRT[m_currentVelRT].bind();
	m_applyForceVelShader.bind();
	m_velRT[m_currentVelRT ^ 1].color().bind(0);
	m_screen.draw();
	m_velRT[m_currentVelRT].unbind();
	//速度発散
	m_divergenceRT.bind();
	m_divergenceShader.bind();
	m_texcelSMP.bind(0);
	m_velRT[m_currentVelRT].color().bind(0);
	m_screen.draw();
	m_divergenceRT.unbind();
	//jacobi法による圧力計算
	for (int i = 0; i < m_numJacobiReps; i++) {
		char currentPressureRT = i % 2;
		m_pressureRT[currentPressureRT].bind();
		m_jacobiPressureShader.bind();
		m_divergenceRT.color().bind(0);
		m_pressureRT[currentPressureRT ^ 1].color().bind(1);
		m_screen.draw();
	}
	//projection
	m_currentVelRT ^= 1;
	m_velRT[m_currentVelRT].bind();
	m_projectionShader.bind();
	m_velRT[m_currentVelRT ^ 1].color().bind(0);
	m_pressureRT[(m_numJacobiReps - 1) % 2].color().bind(1);
	m_screen.draw();
	m_velRT[m_currentVelRT].unbind();
	m_texcelSMP.unbind(0);
	//色・密度移流
	m_colDensRT[m_currentRT].bind();
	m_advColDensShader.bind();
	m_velRT[m_currentVelRT].color().bind(0);
	m_colDensRT[m_currentRT ^ 1].color().bind(1);
	m_screen.draw();
	m_colDensRT[m_currentRT].unbind();

	glViewport(0, 0, width(), height());
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	m_lastShader.bind();
	//m_velRT[m_currentVelRT].color().bind(0);
	m_colDensRT[m_currentRT].color().bind(0);
	//m_pressureRT[(m_numJacobiReps - 1) % 2].color().bind(0);
	m_screen.draw();

	//glViewport(0, 0, width(), height());
	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	//glClear(GL_COLOR_BUFFER_BIT);
	//m_renderDivShader.bind();
	//m_divergenceRT.color().bind(0);
	//m_screen.draw();
}
void FluidSimStage::onShutdown() {}

void FluidSimStage::setStageName()
{
	m_name = "FluidSimStage";
}