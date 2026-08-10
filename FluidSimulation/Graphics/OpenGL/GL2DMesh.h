#pragma once
#include"GL2DMeshData.h"
class GL2DMesh
{
private:
	GLuint m_vao = 0;
	GLuint m_vbo = 0;
	GLuint m_ebo = 0;
	GLsizei m_indexCount = 0;
	GLsizei m_vertexCount = 0;
	GLenum m_primitiveMode = GL_TRIANGLES;
	GLuint m_instanceCount = 0;
public:
	GL2DMesh() = default;
	GL2DMesh(GL2DMeshData& data);
	~GL2DMesh();

	GL2DMesh(GL2DMesh&) = delete;
	GL2DMesh& operator=(GL2DMesh&) = delete;

	GL2DMesh(GL2DMesh&& other) noexcept;
	GL2DMesh& operator=(GL2DMesh&& other) noexcept;

	void create(GL2DMeshData& data, GLuint instanceCount = 1);
	void draw() const;

	[[nodiscard]] GLuint vao() const {
		return m_vao;
	}
	[[nodiscard]] GLuint vbo() const {
		return m_vbo;
	}
	[[nodiscard]] GLuint ebo() const {
		return m_ebo;
	}
	[[nodiscard]] GLuint vertexCount() const {
		return m_vertexCount;
	}
	[[nodiscard]] GLuint indexCount() const {
		return m_indexCount;
	}
	void release();
};