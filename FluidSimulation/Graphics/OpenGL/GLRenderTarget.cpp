#include "pch.h"
#include "GLRenderTarget.h"

GLRenderTarget::GLRenderTarget(int _width, int _height, ColorTexSet cSet) {
	create(_width, _height, cSet);
}

GLRenderTarget::GLRenderTarget(TEXTURE2DDESC texDesc) {
	create(texDesc);
}

GLRenderTarget::~GLRenderTarget() {
	release();
}

GLRenderTarget::GLRenderTarget(GLRenderTarget&& other) noexcept {
	m_fbo = std::move(other.m_fbo);
	m_colorTex = std::move(other.m_colorTex);
	m_depthTex = std::move(other.m_depthTex);
	m_width = std::exchange(other.m_width, 0);
	m_height = std::exchange(other.m_height, 0);
	m_colorTexDesc = std::exchange(other.m_colorTexDesc, TEXTURE2DDESC{});
}
GLRenderTarget& GLRenderTarget::operator=(GLRenderTarget&& other) noexcept {
	if (this != &other) {
		m_fbo = std::move(other.m_fbo);
		m_colorTex = std::move(other.m_colorTex);
		m_depthTex = std::move(other.m_depthTex);
		m_width = std::exchange(other.m_width, 0);
		m_height = std::exchange(other.m_height, 0);
		m_colorTexDesc = std::exchange(other.m_colorTexDesc, TEXTURE2DDESC{});
	}
	return *this;
}

bool GLRenderTarget::create(int _width, int _height, ColorTexSet cSet) {
	//テクスチャ作成
	TEXTURE2DSETTING set;
	set.filter = TEXTURE2DFILTER::LINEAR;
	set.wrap = TEXTURE2DWRAP::CLAMP_TO_EDGE;
	TEXTURE2DDESC desc;
	desc.set = set;
	desc.width = _width;
	desc.height = _height;
	m_width = _width;
	m_height = _height;
	if (cSet == ColorTexSet::NORMAL) {
		desc.internalFormat = GL_RGBA8;
		desc.format = GL_RGBA;
		desc.type = GL_UNSIGNED_BYTE;
	}
	else {
		desc.internalFormat = GL_RGBA16F;
		desc.format = GL_RGBA;
		desc.type = GL_FLOAT;
	}
	m_colorTexDesc = desc;
	m_colorTex.create(desc);
	if (!m_colorTex.valid()) {
		spdlog::critical("faild to create colorTex");
		return false;
	}
	//descは再利用
	desc.set.filter = TEXTURE2DFILTER::NEAREST;
	desc.internalFormat = GL_DEPTH_COMPONENT24;
	desc.format = GL_DEPTH_COMPONENT;
	desc.type = GL_UNSIGNED_INT;
	m_depthTex.create(desc);

	m_fbo.create();
	m_fbo.attachTexture(GL_COLOR_ATTACHMENT0, m_colorTex.id(), 0);
	m_fbo.attachTexture(GL_DEPTH_ATTACHMENT, m_depthTex.id(), 0);

	constexpr GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };

	glDrawBuffers(1, drawBuffers);

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

bool GLRenderTarget::create(TEXTURE2DDESC texDesc) {
	//テクスチャ作成
	m_colorTexDesc = texDesc;
	m_width = m_colorTexDesc.width;
	m_height = m_colorTexDesc.height;
	m_colorTex.create(m_colorTexDesc);
	if (!m_colorTex.valid()) {
		spdlog::critical("faild to create colorTex");
		return false;
	}
	//texDescは再利用
	TEXTURE2DDESC depthDesc = texDesc;
	depthDesc.set.filter = TEXTURE2DFILTER::NEAREST;
	depthDesc.internalFormat = GL_DEPTH_COMPONENT24;
	depthDesc.format = GL_DEPTH_COMPONENT;
	depthDesc.type = GL_UNSIGNED_INT;
	m_depthTex.create(depthDesc);

	m_fbo.create();
	m_fbo.attachTexture(GL_COLOR_ATTACHMENT0, m_colorTex.id(), 0);
	m_fbo.attachTexture(GL_DEPTH_ATTACHMENT, m_depthTex.id(), 0);

	constexpr GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };

	glDrawBuffers(1, drawBuffers);

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

void GLRenderTarget::bind() {
	m_fbo.bind();
}
void GLRenderTarget::unbind() {
	m_fbo.unbind();
}
void GLRenderTarget::resize(int newWidth, int newHeight) {
	//テクスチャ側でreturnシてくれるが一応
	if (newWidth == m_width && newHeight == m_height)
		return;
	//最小化モードのときは変えない
	if (newWidth <= 0 || newHeight <= 0)
		return;

	m_colorTex.resize(newWidth, newHeight);
	m_depthTex.resize(newWidth, newHeight);
	m_width = newWidth;
	m_height = newHeight;
}

void GLRenderTarget::release() {
	if ((m_fbo.id() | m_colorTex.id() | m_depthTex.id()) != 0) {
		m_fbo.release();
		m_colorTex.release();
		m_depthTex.release();
	}
	m_width = 0;
	m_height = 0;
}
