#pragma once
#include<glad/gl.h>
class GLFrameBuffer
{
private:
	GLuint m_id = 0;
public:
	GLFrameBuffer() = default;
	~GLFrameBuffer();

	GLFrameBuffer(GLFrameBuffer&) = delete;
	GLFrameBuffer& operator=(GLFrameBuffer&) = delete;

	GLFrameBuffer(GLFrameBuffer&& other) noexcept;
	GLFrameBuffer& operator=(GLFrameBuffer&& other) noexcept;

	void create();
	void bind();
	void unbind();
	void attachTexture(GLenum attachment, GLuint texture, GLint mipLevel);
	void attachTextureLayer(GLenum attachment, GLuint texture, GLint mipLevel, GLint layer);

	[[nodiscard]] GLuint id() const {
		return m_id;
	}

	[[nodiscard]] bool valid() const {
		return m_id != 0;
	}
	void release();
};

