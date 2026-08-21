#include "pch.h"
#include "Bloom.h"

Bloom::Bloom(int width, int height, float _threshold, float _bloomStrength, int blurStep, float blurScale) {
	create(width, height, _threshold, _bloomStrength, blurStep, blurScale);
}

Bloom::Bloom(Bloom&& other) noexcept {
	m_bloomCompositeShader = std::move(other.m_bloomCompositeShader);
	m_brightPassShader = std::move(other.m_brightPassShader);
	m_blur = std::move(other.m_blur);
	m_bloomCompositeRT = std::move(other.m_bloomCompositeRT);
	m_brightPassPP = std::move(other.m_brightPassPP);
	m_threshold = std::exchange(other.m_threshold, 1.0f);
	m_bloomStrength = std::exchange(other.m_bloomStrength, 1.0f);
}
Bloom& Bloom::operator=(Bloom&& other) noexcept {
	if (this != &other) {
		m_bloomCompositeShader = std::move(other.m_bloomCompositeShader);
		m_brightPassShader = std::move(other.m_brightPassShader);
		m_blur = std::move(other.m_blur);
		m_bloomCompositeRT = std::move(other.m_bloomCompositeRT);
		m_brightPassPP = std::move(other.m_brightPassPP);
		m_threshold = std::exchange(other.m_threshold, 1.0f);
		m_bloomStrength = std::exchange(other.m_bloomStrength, 1.0f);
	}
	return *this;
}

void Bloom::changeThreshold(float newThreshold) {
	if (m_threshold != newThreshold) {
		m_brightPassShader.setUniformFloat("uThreshold", newThreshold);
		m_threshold = newThreshold;
	}
}
void Bloom::changeBloomStrength(float newBloomStrength) {
	if (m_bloomStrength != newBloomStrength) {
		m_bloomCompositeShader.setUniformFloat("uBloomStrength", newBloomStrength);
		m_bloomStrength = newBloomStrength;
	}
}
void Bloom::changeBlurStep(int newBlurStep) {
	m_blur.changeBlurStep(newBlurStep);
}
void Bloom::changeBlurScale(float newBlurScale) {
	m_blur.changeBlurScale(newBlurScale);
}
bool Bloom::create(int width, int height, float _threshold, float _bloomStrength, int blurStep, float blurScale) {
	m_bloomCompositeShader.load("assets\\shaders\\screenVS.glsl", "assets\\shaders\\bloomCompositeFS.glsl");
	if (!m_bloomCompositeShader.valid()) {
		spdlog::critical("faild to load bloom composite shader");
		return false;
	}
	m_brightPassShader.load("assets\\shaders\\screenVS.glsl", "assets\\shaders\\brightPassFS.glsl");
	if (!m_brightPassShader.valid()) {
		spdlog::critical("faild to load bright pass shader");
		return false;
	}
	if (!m_bloomCompositeRT.create(width, height, ColorTexSet::HDR)) {
		spdlog::critical("faild to create bloom composite rt");
		return false;
	}
	if (!m_brightPassPP.create(m_brightPassShader, width, height, ColorTexSet::HDR)) {
		spdlog::critical("faild to create bright pass");
		return false;
	}
	if (!m_blur.create(width, height, blurStep, blurScale)) {
		spdlog::critical("faild to create blur");
		return false;
	}
	changeThreshold(_threshold);
	changeBloomStrength(_bloomStrength);
	return true;
}
void Bloom::resize(int width, int height) {
	m_bloomCompositeRT.resize(width, height);
	m_brightPassPP.resize(width, height);
	m_blur.resize(width, height);
}
const GLTexture2D& Bloom::execute(const GLTexture2D& texture, const GLMesh& screen) {
	m_brightPassPP.execute(texture, screen);
	const GLTexture2D* outTex = &m_brightPassPP.output();

	outTex = &m_blur.execute(*outTex, screen);

	glViewport(0, 0, m_bloomCompositeRT.width(), m_bloomCompositeRT.height());
	m_bloomCompositeRT.bind();
	m_bloomCompositeShader.bind();
	texture.bind(0);
	outTex->bind(1);
	screen.draw();
	m_bloomCompositeRT.unbind();
	outTex = &m_bloomCompositeRT.color();
	return *outTex;
}