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
	m_vortOmegaShader.load("assets\\shaders\\screenVS.glsl", "assets\\shaders\\vortOmegaFS.glsl");
	if (!m_vortOmegaShader.valid()) {
		spdlog::critical("faild to load vort omega Shader");
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
	TEXTURE2DDESC vortOmegaDesc;
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
	vortOmegaDesc = divergenceDesc;		//共通部分はコピー
	for (auto& rt : m_velRT) {
		rt.create(velDesc);
		m_renderTargets.push_back(&rt);
	}
	for (auto& rt : m_colDensRT) {
		rt.create(colDensDesc);
		m_renderTargets.push_back(&rt);
	}
	m_divergenceRT.create(divergenceDesc);
	m_renderTargets.push_back(&m_divergenceRT);
	for (auto& rt : m_pressureRT) {
		rt.create(pressureDesc);
		m_renderTargets.push_back(&rt);
	}
	m_vortOmegaRT.create(vortOmegaDesc);
	m_renderTargets.push_back(&m_vortOmegaRT);

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
	m_FSC.vortStrength = 30;
	m_FSC.densityDecay = 0.0f;
	m_FSC.velocityDecay = 0.0f;
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

	m_initColDensShader.setUniformUInt("enableImgInit", static_cast<uint32_t>(m_enableImgInit));

	glDisable(GL_DEPTH_TEST);
	return true;
}
void FluidSimStage::changeRTResolution(int newWidth, int newHeight) {
	for (auto rt : m_renderTargets) {
		rt->resize(newWidth, newHeight);
	}
}
void FluidSimStage::onUpdate(float delta) {
	m_FSC.time += delta;
	m_FSC.delta = delta;
	m_FSCUB.update(&m_FSC, sizeof(FluidSimConstants), 0);
	if (ImGui::Begin("Fluid Simulator")) {
		ImGui::Text("FPS:%.1f", 1 / delta);
		ImGui::SliderFloat("interaction Force", &m_MS.interactionForce, 0.0f, 15000.0f);
		ImGui::SliderFloat("interaction Radius", &m_MS.interactionRadius, 0.0f, 0.2f);
		ImGui::SliderFloat("vorticity strength", &m_FSC.vortStrength, 0.0f, 50.0f);
		ImGui::SliderFloat("saturation", &m_saturation, 0.0f, 1.0f);
		ImGui::SliderFloat("brightness", &m_brightness, 0.0f, 1.0f);
		ImGui::SliderInt("jacobi pressure repetition", reinterpret_cast<int*>(&m_numJacobiReps), 0, 128);
		ImGui::SliderFloat("density Decay", &m_FSC.densityDecay, 0.0f, 2.0f);
		ImGui::SliderFloat("velocity Decay", &m_FSC.velocityDecay, 0.0f, 2.0f);
		if (ImGui::Button("show gradient")) {
			m_showGrad = !m_showGrad;
		}
		static int newWidth = width();
		static int newHeight = height();
		ImGui::SliderInt("new width", &newWidth, 0.0f, width() * 1.5f);
		ImGui::SliderInt("new height", &newHeight, 0.0f, height() * 1.5f);
		if (ImGui::Button("change resolution")) {
			changeRTResolution(newWidth, newHeight);
			m_shouldReset = true;
		}
		if (ImGui::Checkbox("enable your image reset", &m_enableImgInit)) {
			m_initColDensShader.setUniformUInt("enableImgInit", static_cast<uint32_t>(m_enableImgInit));
		}
		if (ImGui::Button("Reset")) {
			m_shouldReset = true;
		}
	}

	auto mPos = mousePos();
	if (mPos != std::nullopt)
		m_MS.mPos = *mPos / glm::vec2(widthf(), heightf()) * m_FSC.resolution;
	m_MS.mVel = mouseDelta() / delta / glm::vec2(widthf(), heightf()) * m_FSC.resolution;
	m_MS.injectColor = hsb2rgb(m_FSC.time / 5, m_saturation, m_brightness, 1.0f) * 200.0f;
	m_MS.rClick = static_cast<uint32_t>(isMousePress(GLFW_MOUSE_BUTTON_RIGHT));
	m_MS.lClick = static_cast<uint32_t>(isMousePress(GLFW_MOUSE_BUTTON_LEFT));
	ImGui::End();
	m_MSUB.update(&m_MS, sizeof(FluidSimInput), 0);
}
void FluidSimStage::onRender() {
	if (m_shouldReset) {
		glViewport(0, 0, m_velRT[m_currentVelRT].width(), m_velRT[m_currentVelRT].height());
		for (auto& rt : m_velRT) {
			rt.bind();
			m_initVelShader.bind();
			m_screen.draw();
			rt.unbind();
		}
		glViewport(0, 0, m_colDensRT[m_currentColDensRT].width(), m_colDensRT[m_currentColDensRT].height());
		for (auto& rt : m_colDensRT) {
			rt.bind();
			m_initColDensShader.bind();
			m_firstColDens.bind(0);
			m_screen.draw();
			rt.unbind();
		}
		m_shouldReset = false;
	}
	glViewport(0, 0, m_pressureRT[0].width(), m_pressureRT[0].height());
	for (auto& rt : m_pressureRT) {
		rt.bind();
		m_initPressureShader.bind();
		m_screen.draw();
		rt.unbind();
	}
	//速度移流
	m_currentVelRT ^= 1;
	glViewport(0, 0, m_velRT[m_currentVelRT].width(), m_velRT[m_currentVelRT].height());
	m_velRT[m_currentVelRT].bind();
	m_advVelShader.bind();
	m_velRT[m_currentVelRT ^ 1].color().bind(0);
	m_screen.draw();
	m_velRT[m_currentVelRT].unbind();
	//渦度計算
	m_texcelSMP.bind(0);
	glViewport(0, 0, m_vortOmegaRT.width(), m_vortOmegaRT.height());
	m_vortOmegaRT.bind();
	m_vortOmegaShader.bind();
	m_velRT[m_currentVelRT].color().bind(0);
	m_screen.draw();
	m_vortOmegaRT.unbind();
	m_texcelSMP.unbind(0);
	//外力与える
	m_currentVelRT ^= 1;
	glViewport(0, 0, m_velRT[m_currentVelRT].width(), m_velRT[m_currentVelRT].height());
	m_velRT[m_currentVelRT].bind();
	m_applyForceVelShader.bind();
	m_velRT[m_currentVelRT ^ 1].color().bind(0);
	m_vortOmegaRT.color().bind(1);
	m_screen.draw();
	m_velRT[m_currentVelRT].unbind();
	//速度発散
	m_texcelSMP.bind(0);
	glViewport(0, 0, m_divergenceRT.width(), m_divergenceRT.height());
	m_divergenceRT.bind();
	m_divergenceShader.bind();
	m_velRT[m_currentVelRT].color().bind(0);
	m_screen.draw();
	m_divergenceRT.unbind();
	//jacobi法による圧力計算
	glViewport(0, 0, m_pressureRT[0].width(), m_pressureRT[0].height());
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
	glViewport(0, 0, m_velRT[m_currentVelRT].width(), m_velRT[m_currentVelRT].height());
	m_velRT[m_currentVelRT].bind();
	m_projectionShader.bind();
	m_velRT[m_currentVelRT ^ 1].color().bind(0);
	m_pressureRT[(m_numJacobiReps - 1) % 2].color().bind(1);
	m_screen.draw();
	m_velRT[m_currentVelRT].unbind();
	//速度発散
	glViewport(0, 0, m_divergenceRT.width(), m_divergenceRT.height());
	m_divergenceRT.bind();
	m_divergenceShader.bind();
	m_velRT[m_currentVelRT].color().bind(0);
	m_screen.draw();
	m_divergenceRT.unbind();
	m_texcelSMP.unbind(0);
	//色・密度追加
	m_currentColDensRT ^= 1;
	glViewport(0, 0, m_colDensRT[m_currentColDensRT].width(), m_colDensRT[m_currentColDensRT].height());
	m_colDensRT[m_currentColDensRT].bind();
	m_applyInputColDensShader.bind();
	m_colDensRT[m_currentColDensRT ^ 1].color().bind(0);
	m_screen.draw();
	m_colDensRT[m_currentColDensRT].unbind();
	//色・密度移流
	m_currentColDensRT ^= 1;
	glViewport(0, 0, m_colDensRT[m_currentColDensRT].width(), m_colDensRT[m_currentColDensRT].height());
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
	//m_vortOmegaRT.color().bind(0);
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