#pragma once
#include<GaussianBlur.h>
class Bloom
{
private:
	GLShader m_bloomCompositeShader;
	GLShader m_brightPassShader;
	GaussianBlur m_blur;
	GLRenderTarget m_bloomCompositeRT;
	PostProcessPass m_brightPassPP;
	float m_threshold = 1.0f;
	float m_bloomStrength = 1.0f;
public:
	Bloom() = default;
	Bloom(int width, int height, float _threshold = 1.0f, float _bloomStrength = 1.0f, int blurStep = 4, float blurScale = 1.0f, uint32_t _blurRadius = 2, float _sigmaExtent = 3.0f);
	~Bloom() = default;

	Bloom(Bloom&) = delete;
	Bloom& operator=(Bloom&) = delete;

	Bloom(Bloom&& other) noexcept;
	Bloom& operator=(Bloom&& other) noexcept;

	void changeThreshold(float newThreshold);
	void changeBloomStrength(float newBloomStrength);
	void changeBlurStep(int newBlurStep);
	void changeBlurScale(float newBlurScale);
	void changeBlurRadius(uint32_t newBlurRadius);
	void changeSigmaExtent(float newSigmaExtent);
	bool create(int width, int height, float _threshold = 1.0f, float _bloomStrength = 1.0f, int blurStep = 4, float blurScale = 1.0f, uint32_t _blurRadius = 2, float _sigmaExtent = 3.0f);
	void resize(int width, int height);
	const GLTexture2D& execute(const GLTexture2D& texture, const GLMesh& screen);

	[[nodiscard]] float threshold() {
		return m_threshold;
	}
	[[nodiscard]] float bloomStrength() {
		return m_bloomStrength;
	}
	[[nodiscard]] int blurStep() {
		return m_blur.blurStep();
	}
	[[nodiscard]] float blurScale() {
		return m_blur.blurScale();
	}
	[[nodiscard]] uint32_t blurRadius() const {
		return m_blur.blurRadius();
	}
	[[nodiscard]] float sigmaExtent() const {
		return m_blur.sigmaExtent();
	}
};

