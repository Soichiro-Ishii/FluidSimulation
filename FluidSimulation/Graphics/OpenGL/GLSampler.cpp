#include "pch.h"
#include "GLSampler.h"

GLSampler::~GLSampler() {
	release();
}
GLSampler::GLSampler(SAMPLER_DESC _desc) {
	create(_desc);
}

GLSampler::GLSampler(GLSampler&& other) noexcept {
	m_id = std::exchange(other.m_id, 0);
	m_desc = std::exchange(other.m_desc, SAMPLER_DESC{});
}
GLSampler& GLSampler::operator=(GLSampler&& other) noexcept {
	if (this != &other) {
		m_id = std::exchange(other.m_id, 0);
		m_desc = std::exchange(other.m_desc, SAMPLER_DESC{});
	}
	return *this;
}

GLenum tex2DFil2GLenum(TEXTURE2DFILTER filter) {
	if (filter == TEXTURE2DFILTER::NEAREST)
		return GL_NEAREST;
	else
		return GL_LINEAR;
}

GLenum tex2DWrap2GLenum(TEXTURE2DWRAP wrap) {
	if (wrap == TEXTURE2DWRAP::CLAMP_TO_EDGE)
		return GL_CLAMP_TO_EDGE;
	else if (wrap == TEXTURE2DWRAP::REPEAT)
		return GL_REPEAT;
	else
		return GL_MIRRORED_REPEAT;
}

void GLSampler::create(SAMPLER_DESC _desc) {
	glCreateSamplers(1, &m_id);

	glSamplerParameteri(m_id, GL_TEXTURE_MIN_FILTER, tex2DFil2GLenum(_desc.minFilter));
	glSamplerParameteri(m_id, GL_TEXTURE_MAG_FILTER, tex2DFil2GLenum(_desc.magFilter));

	glSamplerParameteri(m_id, GL_TEXTURE_WRAP_S, tex2DWrap2GLenum(_desc.wrapS));
	glSamplerParameteri(m_id, GL_TEXTURE_WRAP_T, tex2DWrap2GLenum(_desc.wrapT));
	glSamplerParameteri(m_id, GL_TEXTURE_WRAP_R, tex2DWrap2GLenum(_desc.wrapR));
}
void GLSampler::bind(GLuint _id) {
	glBindSampler(_id, m_id);
}
void GLSampler::unbind(GLuint _id) {
	glBindSampler(_id, 0);
}

void GLSampler::release() {
	if (m_id != 0) {
		glDeleteSamplers(1, &m_id);
		m_id = 0;
	}
	m_desc = SAMPLER_DESC{};
}
