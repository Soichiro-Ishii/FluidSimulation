#include "pch.h"
#include "GLFrameBuffer.h"

GLFrameBuffer::~GLFrameBuffer() {
	release();
}

GLFrameBuffer::GLFrameBuffer(GLFrameBuffer&& other) noexcept {
	m_id = std::exchange(other.m_id, 0);
}
GLFrameBuffer& GLFrameBuffer::operator=(GLFrameBuffer&& other) noexcept {
	if (this != &other) {
		release();
		m_id = std::exchange(other.m_id, 0);
	}
	return *this;
}

void GLFrameBuffer::create() {
	release();

	glGenFramebuffers(1, &m_id);
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}
void GLFrameBuffer::bind() {
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}
void GLFrameBuffer::unbind() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
void GLFrameBuffer::attachTexture(GLenum attachment, GLuint texture, GLint mipLevel) {
	bind();
	glFramebufferTexture(GL_FRAMEBUFFER, attachment, texture, mipLevel);
}
void GLFrameBuffer::attachTextureLayer(GLenum attachment, GLuint texture, GLint mipLevel, GLint layer) {
	bind();
	glFramebufferTextureLayer(GL_FRAMEBUFFER, attachment, texture, mipLevel, layer);
}
void GLFrameBuffer::release() {
	if (m_id != 0) {
		glDeleteFramebuffers(1, &m_id);
		m_id = 0;
	}
}