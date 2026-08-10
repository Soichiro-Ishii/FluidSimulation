#pragma once
#include<glad/gl.h>
class GLShaderStorageBuffer
{
private:
	GLuint m_id = 0;
	size_t m_size = 0;
	GLuint m_blockIndex = 0;
public:
	GLShaderStorageBuffer() = default;
	GLShaderStorageBuffer(void* data, size_t size, GLuint blockIndex, GLenum usage = GL_DYNAMIC_DRAW);
	~GLShaderStorageBuffer();

	GLShaderStorageBuffer(GLShaderStorageBuffer&) = delete;
	GLShaderStorageBuffer& operator=(GLShaderStorageBuffer&) = delete;

	GLShaderStorageBuffer(GLShaderStorageBuffer&& other) noexcept;
	GLShaderStorageBuffer& operator=(GLShaderStorageBuffer&& other) noexcept;

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

