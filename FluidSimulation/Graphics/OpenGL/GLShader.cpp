#include "pch.h"
#include "GLShader.h"

GLShader::GLShader(const char* vsPath, const char* fsPath) {
	load(vsPath, fsPath);
}

GLShader::~GLShader() {
	release();
}

void GLShader::load(const char* vsPath, const char* fsPath) {
	release();

	LoadStringFile vss(vsPath);
	if (!vss.succeeded())
	{
		spdlog::critical(
			"Failed to load shader file\n"
			"Relative path: {}\n"
			"Absolute path: {}",
			vsPath,
			std::filesystem::absolute(vsPath).string()
		);

		m_id = 0;
		return;
	}
	LoadStringFile fss(fsPath);
	if (!fss.succeeded())
	{
		spdlog::critical(
			"Failed to load shader file\n"
			"Relative path: {}\n"
			"Absolute path: {}",
			fsPath,
			std::filesystem::absolute(fsPath).string()
		);

		m_id = 0;
		return;
	}
	//シェーダーコンパイル
	GLuint vs = CompileShader(GL_VERTEX_SHADER, vss.data());
	if (vs == 0) {
		m_id = 0;
		return;
	}
	GLuint fs = CompileShader(GL_FRAGMENT_SHADER, fss.data());
	if (fs == 0) {
		glDeleteShader(vs);
		m_id = 0;
		return;
	}
	//シェーダープログラム作成
	GLuint program = glCreateProgram();
	//vs,fsセット
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	//リンク
	glLinkProgram(program);
	//結果確認
	GLint linkStatus = GL_FALSE;
	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
	if (linkStatus == GL_FALSE) {
		//ログの長さ取得
		GLint logLen = 0;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLen);
		//ログ取得
		std::string log(logLen, '\0');
		glGetProgramInfoLog(program, logLen, nullptr, log.data());

		spdlog::critical("Shader program link error:\n{}", log);

		glDeleteProgram(program);
		program = 0;
	}
	//vs,fsを削除
	glDeleteShader(vs);
	glDeleteShader(fs);
	m_id = program;
}

void GLShader::bind() const {
	glUseProgram(m_id);
}

GLint GLShader::getUniformLocation(std::string name) const {
	auto it = m_locations.find(name);

	if (it != m_locations.end())
		return it->second;

	GLint loc = glGetUniformLocation(m_id, name.c_str());
	m_locations[name] = loc;
	return loc;
}

void GLShader::setUniformFloat(std::string name, float val) const {
	glProgramUniform1f(m_id, getUniformLocation(name), val);
}
void GLShader::setUniformInt(std::string name, int val) const {
	glProgramUniform1i(m_id, getUniformLocation(name), val);
}
void GLShader::setUniformUInt(std::string name, unsigned int val) const {
	glProgramUniform1ui(m_id, getUniformLocation(name), val);
}

void GLShader::setUniformVec2(std::string name, const glm::vec2& vec) const {
	glProgramUniform2fv(m_id, getUniformLocation(name), 1, &vec.x);
}
void GLShader::setUniformVec3(std::string name, const glm::vec3& vec) const {
	glProgramUniform3fv(m_id, getUniformLocation(name), 1, &vec.x);
}
void GLShader::setUniformVec4(std::string name, const glm::vec4& vec) const {
	glProgramUniform4fv(m_id, getUniformLocation(name), 1, &vec.x);
}

void GLShader::setUniformMat3(std::string name, const glm::mat3& mat) const {
	glProgramUniformMatrix3fv(m_id, getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}
void GLShader::setUniformMat4(std::string name, const glm::mat4& mat) const {
	glProgramUniformMatrix4fv(m_id, getUniformLocation(name), 1, GL_FALSE, &mat[0][0]);
}

GLShader::GLShader(GLShader&& other) noexcept {
	release();
	m_id = std::exchange(other.m_id, 0);
}
GLShader& GLShader::operator=(GLShader&& other) noexcept {
	if (this != &other) {
		release();
		m_id = std::exchange(other.m_id, 0);
	}
	return *this;
}

GLuint GLShader::CompileShader(GLenum shaderType, const char* source) {
	//シェーダー作成
	GLuint shader = glCreateShader(shaderType);
	//シェーダーソースの登録
	glShaderSource(shader, 1, &source, nullptr);
	//コンパイル
	glCompileShader(shader);
	//結果確認
	GLint compileStatus = GL_FALSE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
	//失敗してたらログ表示
	if (compileStatus == GL_FALSE) {
		//ログの長さ取得
		GLint logLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLen);
		//ログ取得
		std::string log(logLen, '\0');
		glGetShaderInfoLog(shader, logLen, nullptr, log.data());

		spdlog::critical("Shader compilation failed:\n{}", log);

		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

void GLShader::release() {
	if (m_id != 0) {
		glDeleteProgram(m_id);
		m_id = 0;
		m_locations.clear();
	}
}