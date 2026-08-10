#include "pch.h"
#include "FluidSimStage.h"
#include"ProcMeshGenerator.h"

/**
 * HSB (HSV) を RGB (glm::vec4) に変換する関数
 *
 * @param h Hue (色相): 0.0 〜 1.0
 * @param s Saturation (彩度): 0.0 〜 1.0
 * @param b Brightness/Value (明度): 0.0 〜 1.0
 * @param a Alpha (透明度): 0.0 〜 1.0 (デフォルト 1.0)
 * @return glm::vec4 (r, g, b, a) 各成分 0.0 〜 1.0
 */
glm::vec4 hsb2rgb(float h, float s, float b, float a = 1.0f) {
	// GLSLでおなじみのベクトル化されたHSB->RGB変換アルゴリズム
	glm::vec3 K = glm::vec3(1.0f, 2.0f / 3.0f, 1.0f / 3.0f);
	glm::vec3 p = glm::abs(glm::fract(glm::vec3(h) + K) * 6.0f - glm::vec3(3.0f));

	glm::vec3 rgb = b * glm::mix(glm::vec3(1.0f), glm::clamp(p - glm::vec3(1.0f), 0.0f, 1.0f), s);

	return glm::vec4(rgb, a);
}

/**
 * hsb2rgb の glm::vec3 入力用オーバーロード (h: 0~1, s: 0~1, b: 0~1)
 */
glm::vec4 hsb2rgb(const glm::vec3& hsb, float a = 1.0f) {
	return hsb2rgb(hsb.x, hsb.y, hsb.z, a);
}

/**
 * Hueを度数 (0°〜360°) で指定したい場合
 */
glm::vec4 hsbDegrees2rgb(float h_deg, float s, float b, float a = 1.0f) {
	return hsb2rgb(h_deg / 360.0f, s, b, a);
}

FluidSimStage::FluidSimStage()
{
	setStageName();
}
bool FluidSimStage::onInit() {

	m_renderTexShader.load("assets\\shaders\\screenVS.glsl", "assets\\shaders\\renderTexFS.glsl");
	if (!m_renderTexShader.valid()) {
		spdlog::critical("faild to load lastShader");
		return false;
	}
	m_renderGradTexShader.load("assets\\shaders\\screenVS.glsl", "assets\\shaders\\renderGradTexFS.glsl");
	if (!m_renderGradTexShader.valid()) {
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
	if (!m_renderDivShader.valid()) {
		spdlog::critical("faild to load render divergence Shader");
		return false;
	}
	m_jacobiPressureShader.load("assets\\shaders\\screenVS.glsl", "assets\\shaders\\jacobiPressureFS.glsl");
	if (!m_jacobiPressureShader.valid()) {
		spdlog::critical("faild to load jacobi pressure Shader");
		return false;
	}
	m_projectionShader.load("assets\\shaders\\screenVS.glsl", "assets\\shaders\\projectionFS.glsl");
	if (!m_projectionShader.valid()) {
		spdlog::critical("faild to load projection Shader");
		return false;
	}
	m_applyForceVelShader.load("assets\\shaders\\screenVS.glsl", "assets\\shaders\\applyForceVelFS.glsl");
	if (!m_applyForceVelShader.valid()) {
		spdlog::critical("faild to load apply force to velocity Shader");
		return false;
	}
	m_applyInputColDensShader.load("assets\\shaders\\screenVS.glsl", "assets\\shaders\\applyInputColDensFS.glsl");
	if (!m_applyInputColDensShader.valid()) {
		spdlog::critical("faild to load apply input to color and density Shader");
		return false;
	}

	TEXTURE2DSETTING firstColorDensSet;
	firstColorDensSet.filter = TEXTURE2DFILTER::LINEAR;
	firstColorDensSet.wrap = TEXTURE2DWRAP::CLAMP_TO_EDGE;
	firstColorDensSet.colorSpace = COLOR_SPACE::SRGB;
	std::string path = "assets\\data\\textures\\blackHole.png";

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
	m_MS.injectColor = glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
	m_MS.rClick = 0;
	m_MS.lClick = 0;
	m_MSUB.create(&m_MS, sizeof(FluidSimInput), 1);

	glDisable(GL_DEPTH_TEST);
	return true;
}
void FluidSimStage::onUpdate(float delta) {
	m_FSC.time += delta;
	m_FSC.delta = delta;
	m_FSCUB.update(&m_FSC, sizeof(FluidSimConstants), 0);
	auto mPos = mousePos();
	if (mPos != std::nullopt)
		m_MS.mPos = *mPos;
	m_MS.mVel = mouseDelta() / delta;
	m_MS.injectColor = hsb2rgb(m_FSC.time, 1.0f, 1.0f, 1.0f);
	m_MS.rClick = static_cast<uint32_t>(isMousePress(GLFW_MOUSE_BUTTON_RIGHT));
	m_MS.lClick = static_cast<uint32_t>(isMousePress(GLFW_MOUSE_BUTTON_LEFT));
	if (ImGui::Begin("Fluid Simulator")) {
		ImGui::SliderFloat("interaction Force", &m_MS.interactionForce, 0.0f, 15000.0f);
		ImGui::SliderFloat("interaction Radius", &m_MS.interactionRadius, 0.0f, 1.0f);
		if (ImGui::Button("show gradient")) {
			m_showGrad = !m_showGrad;
		}
	}
	ImGui::End();
	m_MSUB.update(&m_MS, sizeof(FluidSimInput), 0);
}
void FluidSimStage::onRender() {
	glViewport(0, 0, m_velRT[m_currentVelRT].width(), m_velRT[m_currentVelRT].height());
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
	//速度発散
	m_divergenceRT.bind();
	m_divergenceShader.bind();
	m_velRT[m_currentVelRT].color().bind(0);
	m_screen.draw();
	m_divergenceRT.unbind();
	m_texcelSMP.unbind(0);
	//色・密度追加
	m_currentColDensRT ^= 1;
	m_colDensRT[m_currentColDensRT].bind();
	m_applyInputColDensShader.bind();
	m_colDensRT[m_currentColDensRT ^ 1].color().bind(0);
	m_screen.draw();
	m_colDensRT[m_currentColDensRT].unbind();
	//色・密度移流
	m_currentColDensRT ^= 1;
	m_colDensRT[m_currentColDensRT].bind();
	m_advColDensShader.bind();
	m_velRT[m_currentVelRT].color().bind(0);
	m_colDensRT[m_currentColDensRT ^ 1].color().bind(1);
	m_screen.draw();
	m_colDensRT[m_currentColDensRT].unbind();

	glViewport(0, 0, width(), height());
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	if (m_showGrad)
		m_renderGradTexShader.bind();
	else
		m_renderTexShader.bind();
	//m_velRT[m_currentVelRT].color().bind(0);
	m_colDensRT[m_currentColDensRT].color().bind(0);
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