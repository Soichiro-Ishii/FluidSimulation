#include"pch.h"
#include "GLMesh.h"

GLMesh::GLMesh(GLMeshData& data) {
	create(data);
}
GLMesh::~GLMesh() {
	release();
}

GLMesh::GLMesh(GLMesh&& other) noexcept {
	m_vao = std::exchange(other.m_vao, 0);
	m_vbo = std::exchange(other.m_vbo, 0);
	m_ebo = std::exchange(other.m_ebo, 0);
	m_vertexCount = std::exchange(other.m_vertexCount, 0);
	m_indexCount = std::exchange(other.m_indexCount, 0);
	m_primitiveMode = std::exchange(other.m_primitiveMode, GL_TRIANGLES);
	m_instanceCount = std::exchange(other.m_instanceCount, 1);
}
GLMesh& GLMesh::operator=(GLMesh&& other) noexcept {
	if (this != &other) {
		release();
		m_vao = std::exchange(other.m_vao, 0);
		m_vbo = std::exchange(other.m_vbo, 0);
		m_ebo = std::exchange(other.m_ebo, 0);
		m_vertexCount = std::exchange(other.m_vertexCount, 0);
		m_indexCount = std::exchange(other.m_indexCount, 0);
		m_primitiveMode = std::exchange(other.m_primitiveMode, GL_TRIANGLES);
		m_instanceCount = std::exchange(other.m_instanceCount, 1);
	}
	return *this;
}

void GLMesh::create(GLMeshData& data, GLuint instanceCount) {
	release();
	m_instanceCount = instanceCount;
	//vao作成
	glGenVertexArrays(1, &m_vao);
	//バッファ生成
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	//vaoバインド
	glBindVertexArray(m_vao);

	m_vertexCount = static_cast<GLsizei>(data.vertices.size());
	m_indexCount = static_cast<GLsizei>(data.indices.size());
	m_primitiveMode = data.primitiveMode;

	//vboをバインド
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	//バッファにデータコピー
	glBufferData(
		GL_ARRAY_BUFFER,
		data.vertices.size() * sizeof(Vertex),
		data.vertices.data(),
		GL_STATIC_DRAW
	);

	//eboバインド
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	//コピー
	glBufferData(
		GL_ELEMENT_ARRAY_BUFFER,
		data.indices.size() * sizeof(uint32_t),
		data.indices.data(),
		GL_STATIC_DRAW
	);

	//頂点属性0登録
	glVertexAttribPointer(
		0,							//属性変数の位置
		3,							//成分数(float3)
		GL_FLOAT,					//型
		GL_FALSE,					//整数データの正規化
		sizeof(Vertex),				// 次の頂点までの間隔（stride）
		reinterpret_cast<void*>(
			offsetof(Vertex, position)
			)						//VBO内のバイトオフセット
	);
	//属性0を有効
	glEnableVertexAttribArray(0);
	//頂点属性1登録
	glVertexAttribPointer(
		1,							//属性変数の位置
		3,							//成分数(float3)
		GL_FLOAT,					//型
		GL_FALSE,					//整数データの正規化
		sizeof(Vertex),				// 次の頂点までの間隔（stride）
		reinterpret_cast<void*>(
			offsetof(Vertex, normal)
			)						//VBO内のバイトオフセット
	);
	//属性1を有効
	glEnableVertexAttribArray(1);
	//頂点属性2登録
	glVertexAttribPointer(
		2,							//属性変数の位置
		3,							//成分数(float3)
		GL_FLOAT,					//型
		GL_FALSE,					//整数データの正規化
		sizeof(Vertex),				// 次の頂点までの間隔（stride）
		reinterpret_cast<void*>(
			offsetof(Vertex, tangent)
			)						//VBO内のバイトオフセット
	);
	//属性2を有効
	glEnableVertexAttribArray(2);
	//頂点属性3登録
	glVertexAttribPointer(
		3,							//属性変数の位置
		2,							//成分数(float2)
		GL_FLOAT,					//型
		GL_FALSE,					//整数データの正規化
		sizeof(Vertex),				// 次の頂点までの間隔（stride）
		reinterpret_cast<void*>(
			offsetof(Vertex, uv)
			)						//VBO内のバイトオフセット
	);
	//属性3を有効
	glEnableVertexAttribArray(3);
	//バインド解除
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	spdlog::info("created mesh vertex:{} index:{} instance:{}", m_vertexCount, m_indexCount, m_instanceCount);
}
void GLMesh::draw() const {
	//頂点をセット
	glBindVertexArray(m_vao);
	//描画 インデックスに応じてインデックスでの描画か変える
	if (m_indexCount > 0) {
		glDrawElementsInstanced(
			m_primitiveMode,
			m_indexCount,
			GL_UNSIGNED_INT,
			reinterpret_cast<void*>(0),
			m_instanceCount
		);
	}
	else {
		glDrawArraysInstanced(
			m_primitiveMode,
			0,
			m_vertexCount,
			m_instanceCount
		);
	}
	//バインド解除
	glBindVertexArray(0);
}
void GLMesh::release() {
	if ((m_vao | m_vbo | m_ebo) != 0) {
		glDeleteBuffers(1, &m_ebo);
		m_ebo = 0;
		glDeleteBuffers(1, &m_vbo);
		m_vbo = 0;
		glDeleteVertexArrays(1, &m_vao);
		m_vao = 0;
	}
	m_vertexCount = 0;
	m_indexCount = 0;
	m_primitiveMode = GL_TRIANGLES;
	m_instanceCount = 0;
}
