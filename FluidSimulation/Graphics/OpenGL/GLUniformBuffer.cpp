#include "pch.h"
#include "GLUniformBuffer.h"

GLUniformBuffer::GLUniformBuffer(void* data, size_t size, GLuint blockIndex, GLenum usage) {
	create(data, size, blockIndex, usage);
}

GLUniformBuffer::~GLUniformBuffer() {
	release();
}

GLUniformBuffer::GLUniformBuffer(GLUniformBuffer&& other) noexcept {
	m_id = std::exchange(other.m_id, 0);
	m_size = std::exchange(other.m_size, 0);
	m_blockIndex = std::exchange(other.m_blockIndex, 0);
}


GLUniformBuffer& GLUniformBuffer::operator=(GLUniformBuffer&& other) noexcept {
	if (this != &other) {
		release();
		m_id = std::exchange(other.m_id, 0);
		m_size = std::exchange(other.m_size, 0);
		m_blockIndex = std::exchange(other.m_blockIndex, 0);
	}
	return *this;
}

void GLUniformBuffer::create(void* data, size_t size, GLuint blockIndex, GLenum usage) {
	release();
	//バッファ生成
	glGenBuffers(1, &m_id);
	m_size = size;
	m_blockIndex = blockIndex;
	//uboバインド
	glBindBuffer(GL_UNIFORM_BUFFER, m_id);
	//コピー
	glBufferData(
		GL_UNIFORM_BUFFER,
		m_size,
		data,
		usage
	);

	//レイアウト設定
	glBindBufferBase(
		GL_UNIFORM_BUFFER,
		m_blockIndex,
		m_id
	);
}
void GLUniformBuffer::update(void* data, size_t size, size_t offset) {
	if (m_id == 0) {
		return;
	}
	if (offset + size > m_size) {
		spdlog::error(
			"UniformBuffer update out of range: offset={}, size={}, capacity={}",
			offset,
			size,
			m_size
		);
		return;
	}
	//uboバインド
	glBindBuffer(GL_UNIFORM_BUFFER, m_id);
	//uboにコピー
	glBufferSubData(
		GL_UNIFORM_BUFFER,
		offset,
		size,
		data
	);
}
void GLUniformBuffer::bindBufferBase() {
	glBindBufferBase(
		GL_UNIFORM_BUFFER,
		m_blockIndex,
		m_id
	);
}
void GLUniformBuffer::changeBlockIndex(GLuint blockIndex) {
	m_blockIndex = blockIndex;
}

void GLUniformBuffer::release() {
	if (m_id != 0) {
		glDeleteBuffers(1, &m_id);
		m_id = 0;
	}
	m_blockIndex = 0;
	m_size = 0;
}
