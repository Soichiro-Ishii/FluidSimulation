#pragma once
#include<glad/gl.h>
class GLCompute
{
private:
	GLuint m_id = 0;
	GLuint m_groupCountX = 0;
	GLuint m_groupCountY = 0;
	GLuint m_groupCountZ = 0;
public:
	GLCompute() = default;
	GLCompute(const char* csPath, GLuint x, GLuint y = 1, GLuint z = 1);
	~GLCompute();
	GLCompute(const GLCompute&) = delete;
	GLCompute& operator=(const GLCompute&) = delete;
	GLCompute(GLCompute&& other) noexcept;
	GLCompute& operator=(GLCompute&& other) noexcept;
	void load(const char* csPath, GLuint x, GLuint y = 1, GLuint z = 1);
	void dispatch() const;
	[[nodiscard]] GLuint id() const
	{
		return m_id;
	}

	[[nodiscard]] bool valid() const
	{
		return m_id != 0;
	}
	void release();//解放
private:
	static GLuint CompileShader(GLenum shaderType, const char* source);	//シェーダーコンパイル
};

