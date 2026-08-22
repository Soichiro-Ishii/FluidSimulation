#include "pch.h"
#include "GaussianBlur.h"

GaussianBlur::GaussianBlur(int width, int height, int _blurStep, float _blurScale, uint32_t _blurRadius, float _sigmaExtent) {
	create(width, height, _blurStep, _blurScale, _blurRadius, _sigmaExtent);
}

GaussianBlur::GaussianBlur(GaussianBlur&& other) noexcept {
	m_blurStep = std::exchange(other.m_blurStep, 4);
	m_shader = std::move(other.m_shader);
	for (int i = 0; i < 2; i++)
		m_PP[i] = std::move(other.m_PP[i]);
	m_PPC = std::move(m_PPC);
	m_blurScale = std::exchange(m_blurScale, 1.0f);
}
GaussianBlur& GaussianBlur::operator=(GaussianBlur&& other) noexcept {
	if (this != &other) {
		m_blurStep = std::exchange(other.m_blurStep, 4);
		m_shader = std::move(other.m_shader);
		for (int i = 0; i < 2; i++)
			m_PP[i] = std::move(other.m_PP[i]);
		m_PPC = std::move(m_PPC);
		m_blurScale = std::exchange(m_blurScale, 1.0f);
	}
	return *this;
}

bool GaussianBlur::create(int width, int height, int _blurStep, float _blurScale, uint32_t _blurRadius, float _sigmaExtent) {
	if (_blurStep <= 0) {
		spdlog::critical("Gaussian blur init:Num blur repetition is {}. It is invalid!!!", _blurStep);
		return false;
	}
	m_shader.load("assets\\shaders\\screenVS.glsl", "assets\\shaders\\blurFS.glsl");
	if (!m_shader.valid()) {
		spdlog::critical("faild to load blurShader");
		return false;
	}
	m_shader.setUniformFloat("uBlurScale", m_blurScale);
	for (auto& rt : m_PP) {
		if (!rt.create(m_shader, width, height, ColorTexSet::HDR)) {
			spdlog::critical("faild to create blur process");
			return false;
		}
	}
	m_PPC.allocate(_blurStep);
	for (int i = 0; i < _blurStep; i++) {
		m_PPC.add(m_PP[i % 2]);
	}
	m_blurStep = _blurStep;
	m_blurScale = _blurScale;
	m_blurRadius = std::max(_blurRadius, 1u);
	m_sigmaExtent = std::max(_sigmaExtent, 0.001f);

	m_shader.setUniformFloat("uBlurScale", m_blurScale);
	m_shader.setUniformUInt("uRadius", m_blurRadius);
	m_shader.setUniformFloat("uSigmaExtent", m_sigmaExtent);
	return true;
}
void GaussianBlur::changeBlurStep(int newRep) {
	if (newRep <= 0) {
		spdlog::critical("Gaussian blur change repetition:Num blur repetition is {}. It is invalid!!!", newRep);
		return;
	}
	m_blurStep = newRep;
	m_PPC.clear();
	m_PPC.allocate(m_blurStep);
	for (int i = 0; i < m_blurStep; i++) {
		m_PPC.add(m_PP[i % 2]);
	}
}
void GaussianBlur::changeBlurScale(float newBlurScale) {
	if (m_blurScale != newBlurScale) {
		m_shader.setUniformFloat("uBlurScale", newBlurScale);
		m_blurScale = newBlurScale;
	}
}
void GaussianBlur::changeBlurRadius(uint32_t newBlurRadius)
{
	if (m_blurRadius != newBlurRadius) {
		m_shader.setUniformUInt("uRadius", newBlurRadius);
		m_blurRadius = newBlurRadius;
	}

}
void GaussianBlur::changeSigmaExtent(float newSigmaExtent)
{
	if (m_sigmaExtent != newSigmaExtent) {
		m_shader.setUniformFloat("uSigmaExtent", newSigmaExtent);
		m_sigmaExtent = newSigmaExtent;
	}
}
void GaussianBlur::resize(int width, int height) {
	m_PPC.resize(width, height);
}
const GLTexture2D& GaussianBlur::execute(const GLTexture2D& texture, const GLMesh& screen) {
	return m_PPC.execute(texture, screen);
}
