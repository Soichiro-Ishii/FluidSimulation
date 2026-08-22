#pragma once
#include<PostProcessPass.h>
#include<PostProcessChain.h>
#include<GLShader.h>
class GaussianBlur
{
private:
	int m_blurStep = 4;
	GLShader m_shader;
	PostProcessPass m_PP[2];
	PostProcessChain m_PPC;
	float m_blurScale = 1.0f;
	uint32_t m_blurRadius = 2;
	float m_sigmaExtent = 3.0f;
public:
	GaussianBlur() = default;
	GaussianBlur(int width, int height, int _blurStep = 4, float _blurScale = 1.0f, uint32_t _blurRadius = 2, float _sigmaExtent = 3.0f);
	~GaussianBlur() = default;

	GaussianBlur(GaussianBlur&) = delete;
	GaussianBlur& operator=(GaussianBlur&) = delete;

	GaussianBlur(GaussianBlur&& other) noexcept;
	GaussianBlur& operator=(GaussianBlur&& other) noexcept;

	bool create(int width, int height, int _blurStep = 4, float _blurScale = 1.0f, uint32_t _blurRadius = 2, float _sigmaExtent = 3.0f);
	void changeBlurStep(int newRep);
	void changeBlurScale(float newBlurScale);
	void changeBlurRadius(uint32_t newBlurRadius);
	void changeSigmaExtent(float newSigmaExtent);
	void resize(int width, int height);
	const GLTexture2D& execute(const GLTexture2D& texture, const GLMesh& screen);

	[[nodiscard]] int blurStep() const {
		return m_blurStep;
	}
	[[nodiscard]] float blurScale() const {
		return m_blurScale;
	}
	[[nodiscard]] int blurRadius() const {
		return m_blurRadius;
	}
	[[nodiscard]] float sigmaExtent() const {
		return m_sigmaExtent;
	}
};

