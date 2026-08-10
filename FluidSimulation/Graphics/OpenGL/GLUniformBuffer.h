#pragma once
#include <glad/gl.h>
#include <cstddef>
class GLUniformBuffer
{
private:
	GLuint m_id = 0;
	size_t m_size = 0;
	GLuint m_blockIndex = 0;
public:
	GLUniformBuffer() = default;
	GLUniformBuffer(void* data, size_t size, GLuint blockIndex, GLenum usage = GL_DYNAMIC_DRAW);
	~GLUniformBuffer();

	GLUniformBuffer(GLUniformBuffer&) = delete;
	GLUniformBuffer& operator=(GLUniformBuffer&) = delete;

	GLUniformBuffer(GLUniformBuffer&& other) noexcept;
	GLUniformBuffer& operator=(GLUniformBuffer&& other) noexcept;

	void create(void* data, size_t size, GLuint blockIndex, GLenum usage = GL_DYNAMIC_DRAW);
	void update(void* data, size_t size, size_t offset);
	void bindBufferBase();
	void changeBlockIndex(GLuint blockIndex);
	[[nodiscard]] GLuint id() const
	{
		return m_id;
	}

	[[nodiscard]] bool valid() const
	{
		return m_id != 0;
	}

	[[nodiscard]] size_t size() const
	{
		return m_size;
	}

	[[nodiscard]] size_t blockIndex() const
	{
		return m_blockIndex;
	}
	void release();
};

