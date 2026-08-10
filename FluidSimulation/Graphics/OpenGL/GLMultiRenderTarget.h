#pragma once
#include"GLFrameBuffer.h"
#include"GLTexture2D.h"
#include"GLRenderTargetTypes.h"
#include<vector>
class GLMultiRenderTarget
{
private:
	GLFrameBuffer m_fbo;
	std::vector<GLTexture2D> m_colorTextures;
	GLTexture2D m_depthTex;
	std::vector<int> m_widths;
	std::vector<int> m_heights;
	std::vector<TEXTURE2DDESC> m_colorTexDescs;
	int m_numAttachments = 0;
public:
	GLMultiRenderTarget() = default;
	GLMultiRenderTarget(int _width, int _height, std::vector<ColorTexSet>& cSet);
	~GLMultiRenderTarget();

	GLMultiRenderTarget(GLMultiRenderTarget&) = delete;
	GLMultiRenderTarget& operator=(GLMultiRenderTarget&) = delete;

	GLMultiRenderTarget(GLMultiRenderTarget&& other) noexcept;
	GLMultiRenderTarget& operator=(GLMultiRenderTarget&& other) noexcept;

	bool create(int _width, int _height, std::vector<ColorTexSet>& cSet);
	bool create(std::vector<TEXTURE2DDESC>& texDesc);
	void bind();
	void unbind();
	void resize(int width, int height);
	void resize(std::vector<int> widths, std::vector<int> heights);

	[[nodiscard]] GLuint fboId() const {
		return m_fbo.id();
	}
	[[nodiscard]] GLuint colorId(int id = 0) const {
		if (id >= m_numAttachments) {
			spdlog::critical("The colorTexture for that ID does not exist.");
			return 0;
		}
		return m_colorTextures[id].id();
	}
	[[nodiscard]] GLuint depthId() const {
		return m_depthTex.id();
	}
	[[nodiscard]] const GLTexture2D& color(int id = 0) const {
		return m_colorTextures.at(id);
	}
	[[nodiscard]] const GLTexture2D& depth() const {
		return m_depthTex;
	}

	[[nodiscard]] int width(int id = 0) const {
		if (id >= m_numAttachments) {
			spdlog::critical("The colorTexture for that ID does not exist.");
			return 0;
		}
		return m_widths[id];
	}
	[[nodiscard]] int height(int id = 0) const {
		if (id >= m_numAttachments) {
			spdlog::critical("The colorTexture for that ID does not exist.");
			return 0;
		}
		return m_heights[id];
	}

	void release();
};

