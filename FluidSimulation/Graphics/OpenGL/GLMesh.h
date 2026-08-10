#pragma once
#include"GLMeshData.h"
class GLMesh
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
	GLMesh() = default;
	GLMesh(GLMeshData& data);
	~GLMesh();

	GLMesh(GLMesh&) = delete;
	GLMesh& operator=(GLMesh&) = delete;

	GLMesh(GLMesh&& other) noexcept;
	GLMesh& operator=(GLMesh&& other) noexcept;

	void create(GLMeshData& data, GLuint instanceCount = 1);
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