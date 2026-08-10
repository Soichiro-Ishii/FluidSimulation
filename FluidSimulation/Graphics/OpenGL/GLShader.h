#pragma once
#include <glad/gl.h>
#include <string>
#include<unordered_map>

class GLShader
{
private:
	GLuint m_id = 0;
	mutable std::unordered_map<std::string, GLint> m_locations;
public:
	GLShader() = default;
	GLShader(const char* vsPath, const char* fsPath);
	~GLShader();
	GLShader(const GLShader&) = delete;
	GLShader& operator=(const GLShader&) = delete;
	GLShader(GLShader&& other) noexcept;
	GLShader& operator=(GLShader&& other) noexcept;
	void load(const char* vsPath, const char* fsPath);
	void bind() const;
	GLint getUniformLocation(std::string name) const;

	void setUniformFloat(std::string name, float val) const;
	void setUniformInt(std::string name, int val) const;
	void setUniformUInt(std::string name, unsigned int val) const;

	void setUniformVec2(std::string name, const glm::vec2& vec) const;
	void setUniformVec3(std::string name, const glm::vec3& vec) const;
	void setUniformVec4(std::string name, const glm::vec4& vec) const;

	void setUniformMat3(std::string name, const glm::mat3& mat) const;
	void setUniformMat4(std::string name, const glm::mat4& mat) const;

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

