#include "pch.h"
#include "GLMultiRenderTarget.h"

GLMultiRenderTarget::GLMultiRenderTarget(int _width, int _height, std::vector<ColorTexSet>& cSet) {
	create(_width, _height, cSet);
}
GLMultiRenderTarget::~GLMultiRenderTarget() {
	release();
}

GLMultiRenderTarget::GLMultiRenderTarget(GLMultiRenderTarget&& other) noexcept {
	m_fbo = std::move(other.m_fbo);
	m_colorTextures = std::move(other.m_colorTextures);
	m_depthTex = std::move(other.m_depthTex);
	m_widths = std::move(other.m_widths);
	m_heights = std::move(other.m_heights);
	m_colorTexDescs = std::move(other.m_colorTexDescs);
	m_numAttachments = std::exchange(other.m_numAttachments, 0);
}
GLMultiRenderTarget& GLMultiRenderTarget::operator=(GLMultiRenderTarget&& other) noexcept {
	if (this != &other) {
		m_fbo = std::move(other.m_fbo);
		m_colorTextures = std::move(other.m_colorTextures);
		m_depthTex = std::move(other.m_depthTex);
		m_widths = std::move(other.m_widths);
		m_heights = std::move(other.m_heights);
		m_colorTexDescs = std::move(other.m_colorTexDescs);
		m_numAttachments = std::exchange(other.m_numAttachments, 0);
	}
	return *this;
}

bool GLMultiRenderTarget::create(int _width, int _height, std::vector<ColorTexSet>& cSet) {
	GLint maxAttachments = 0;
	m_numAttachments = cSet.size();
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxAttachments);
	spdlog::info("max Attachments : {}", maxAttachments);
	if (m_numAttachments > maxAttachments) {
		spdlog::critical("Num of max attachments is {}. {} attachments is too many.", maxAttachments, m_numAttachments);
		return false;
	}
	if (m_numAttachments <= 0) {
		spdlog::critical("Num of min attachments is 1. {} attachments is too low.", m_numAttachments);
		return false;
	}
	//テクスチャ作成
	TEXTURE2DSETTING set;
	set.filter = TEXTURE2DFILTER::LINEAR;
	set.wrap = TEXTURE2DWRAP::CLAMP_TO_EDGE;
	TEXTURE2DDESC desc;
	desc.set = set;
	desc.width = _width;
	desc.height = _height;
	m_colorTextures.resize(m_numAttachments);
	m_colorTexDescs.resize(m_numAttachments);
	m_widths.resize(m_numAttachments);
	m_heights.resize(m_numAttachments);
	for (int i = 0; i < m_numAttachments; i++) {
		m_widths[i] = _width;
		m_heights[i] = _height;
		if (cSet[i] == ColorTexSet::NORMAL) {
			desc.internalFormat = GL_RGBA8;
			desc.format = GL_RGBA;
			desc.type = GL_UNSIGNED_BYTE;
		}
		else {
			desc.internalFormat = GL_RGBA16F;
			desc.format = GL_RGBA;
			desc.type = GL_FLOAT;
		}
		m_colorTexDescs[i] = desc;
		m_colorTextures[i].create(desc);
		if (!m_colorTextures[i].valid()) {
			spdlog::critical("faild to create colorTex{}", i);
			return false;
		}
	}
	//descは再利用
	desc.set.filter = TEXTURE2DFILTER::NEAREST;
	desc.internalFormat = GL_DEPTH_COMPONENT24;
	desc.format = GL_DEPTH_COMPONENT;
	desc.type = GL_UNSIGNED_INT;
	m_depthTex.create(desc);

	m_fbo.create();
	std::vector<GLenum> colorAttachments;
	colorAttachments.resize(m_numAttachments);
	for (int i = 0; i < m_numAttachments; i++) {
		GLenum attachment = GL_COLOR_ATTACHMENT0 + i;
		m_fbo.attachTexture(attachment, m_colorTextures[i].id(), 0);
		colorAttachments[i] = attachment;
	}
	m_fbo.attachTexture(GL_DEPTH_ATTACHMENT, m_depthTex.id(), 0);

	glDrawBuffers(static_cast<GLsizei>(m_numAttachments), colorAttachments.data());

	const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		spdlog::error(
			"Framebuffer incomplete: 0x{:X}",
			static_cast<unsigned int>(status)
		);

		release();
		return false;
	}

	m_fbo.unbind();
	return true;
}

bool GLMultiRenderTarget::create(std::vector<TEXTURE2DDESC>& texDesc) {
	GLint maxAttachments = 0;
	m_numAttachments = texDesc.size();
	glGetIntegerv(GL_MAX_COLOR_ATTACHMENTS, &maxAttachments);
	if (m_numAttachments > maxAttachments) {
		spdlog::critical("Num of max attachments is 8. {} attachments is too many.", m_numAttachments);
		return false;
	}
	if (m_numAttachments <= 0) {
		spdlog::critical("Num of min attachments is 1. {} attachments is too low.", m_numAttachments);
		return false;
	}
	//テクスチャ作成
	m_colorTextures.resize(m_numAttachments);
	m_colorTexDescs.resize(m_numAttachments);
	m_widths.resize(m_numAttachments);
	m_heights.resize(m_numAttachments);
	m_colorTexDescs = texDesc;
	for (int i = 0; i < m_numAttachments; i++) {
		m_widths[i] = m_colorTexDescs[i].width;
		m_heights[i] = m_colorTexDescs[i].height;
		m_colorTextures[i].create(m_colorTexDescs[i]);
		if (!m_colorTextures[i].valid()) {
			spdlog::critical("faild to create colorTex{}", i);
			return false;
		}
	}
	//texDescは再利用 一応0番を使う
	TEXTURE2DDESC depthDesc = texDesc[0];
	depthDesc.set.filter = TEXTURE2DFILTER::NEAREST;
	depthDesc.internalFormat = GL_DEPTH_COMPONENT24;
	depthDesc.format = GL_DEPTH_COMPONENT;
	depthDesc.type = GL_UNSIGNED_INT;
	m_depthTex.create(depthDesc);

	m_fbo.create();
	std::vector<GLenum> colorAttachments;
	colorAttachments.resize(m_numAttachments);
	for (int i = 0; i < m_numAttachments; i++) {
		GLenum attachment = GL_COLOR_ATTACHMENT0 + i;
		m_fbo.attachTexture(attachment, m_colorTextures[i].id(), 0);
		colorAttachments[i] = attachment;
	}
	m_fbo.attachTexture(GL_DEPTH_ATTACHMENT, m_depthTex.id(), 0);

	glDrawBuffers(static_cast<GLsizei>(m_numAttachments), colorAttachments.data());

	const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (status != GL_FRAMEBUFFER_COMPLETE) {
		spdlog::error(
			"Framebuffer incomplete: 0x{:X}",
			static_cast<unsigned int>(status)
		);

		release();
		return false;
	}

	m_fbo.unbind();
	return true;
}

void GLMultiRenderTarget::bind() {
	m_fbo.bind();
}
void GLMultiRenderTarget::unbind() {
	m_fbo.unbind();
}
void GLMultiRenderTarget::resize(int newWidth, int newHeight) {
	if (newWidth == m_widths[0] && newHeight == m_heights[0])
		return;
	//最小化モードのときは変えない
	if (newWidth <= 0 || newHeight <= 0)
		return;
	for (auto& colorTex : m_colorTextures)
		colorTex.resize(newWidth, newHeight);
	m_depthTex.resize(newWidth, newHeight);
	for (auto& width : m_widths)
		width = newWidth;
	for (auto& height : m_heights)
		height = newHeight;
}

void GLMultiRenderTarget::resize(std::vector<int> newWidths, std::vector<int> newHeights) {
	//新しい高さと幅の数が同じか、新しい幅と現在の幅の数が同じが確かめる
	if (newWidths.size() != newHeights.size() || newWidths.size() != m_widths.size()) {
		return;
	}
	//最小化モードのときは変えない
	for (int i = 0; i < newWidths.size(); i++) {
		if (newWidths[i] <= 0 || newHeights[i] <= 0)
			return;
	}
	for (int i = 0; i < newWidths.size(); i++) {
		m_colorTextures[i].resize(newWidths[i], newHeights[i]);
	}
	m_depthTex.resize(newWidths[0], newHeights[0]);
	m_widths = newWidths;
	m_heights = newHeights;
}

void GLMultiRenderTarget::release() {
	GLuint idOrs = 0;
	for (auto& colorTex : m_colorTextures)
		idOrs |= colorTex.id();
	if ((m_fbo.id() | idOrs | m_depthTex.id()) != 0) {
		m_fbo.release();
		for (auto& colorTex : m_colorTextures)
			colorTex.release();
		m_depthTex.release();
	}
	for (auto& width : m_widths)
		width = 0;
	for (auto& height : m_heights)
		height = 0;
}
