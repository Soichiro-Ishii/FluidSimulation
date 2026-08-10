#pragma once
#include"GLFrameBuffer.h"
#include"GLTexture2D.h"
#include"GLRenderTargetTypes.h"

class GLRenderTarget
{
private:
	GLFrameBuffer m_fbo;
	GLTexture2D m_colorTex;
	GLTexture2D m_depthTex;
	int m_width = 0;
	int m_height = 0;
	TEXTURE2DDESC m_colorTexDesc;
public:
	GLRenderTarget() = default;
	GLRenderTarget(int _width, int _height, ColorTexSet cSet);
	GLRenderTarget(TEXTURE2DDESC texDesc);
	~GLRenderTarget();

	GLRenderTarget(GLRenderTarget&) = delete;
	GLRenderTarget& operator=(GLRenderTarget&) = delete;

	GLRenderTarget(GLRenderTarget&& other) noexcept;
	GLRenderTarget& operator=(GLRenderTarget&& other) noexcept;

	bool create(int _width, int _height, ColorTexSet cSet);
	bool create(TEXTURE2DDESC texDesc);
	void bind();
	void unbind();
	void resize(int width, int height);

	[[nodiscard]] GLuint fboId() const {
		return m_fbo.id();
	}
	[[nodiscard]] GLuint colorId() const {
		return m_colorTex.id();
	}
	[[nodiscard]] GLuint depthId() const {
		return m_depthTex.id();
	}
	[[nodiscard]] const GLTexture2D& color() const {
		return m_colorTex;
	}
	[[nodiscard]] const GLTexture2D& depth() const {
		return m_depthTex;
	}

	[[nodiscard]] int width() const {
		return m_width;
	}
	[[nodiscard]] int height() const {
		return m_height;
	}

	void release();
};

