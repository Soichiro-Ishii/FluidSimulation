#include "pch.h"
#include "GLCompute.h"

GLCompute::GLCompute(const char* csPath, GLuint x, GLuint y, GLuint z) {
	load(csPath, x, y, z);
}

GLCompute::~GLCompute() {
	release();
}

void GLCompute::load(const char* csPath, GLuint x, GLuint y, GLuint z) {
	release();

	LoadStringFile css(csPath);
	if (!css.succeeded())
	{
		spdlog::critical(
			"Failed to load shader file\n"
			"Relative path: {}\n"
			"Absolute path: {}",
			csPath,
			std::filesystem::absolute(csPath).string()
		);

		m_id = 0;
		return;
	}
	GLuint cs = CompileShader(GL_COMPUTE_SHADER, css.data());
	if (cs == 0) {
		m_id = 0;
		return;
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, cs);
	glLinkProgram(program);

	GLint linkStatus = GL_FALSE;

	glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);

	if (linkStatus != GL_TRUE)
	{
		GLint logLength = 0;

		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

		std::string log(
			static_cast<std::size_t>(logLength),
			'\0'
		);

		glGetProgramInfoLog(program, logLength, nullptr, log.data());

		spdlog::error(
			"Compute program link error:\n{}",
			log
		);

		glDeleteProgram(program);
		program = 0;
	}
	glDeleteShader(cs);

	m_id = program;
	m_groupCountX = x;
	m_groupCountY = y;
	m_groupCountZ = z;
}

void GLCompute::dispatch() const {
	glUseProgram(m_id);
	glDispatchCompute(m_groupCountX, m_groupCountY, m_groupCountZ);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

GLCompute::GLCompute(GLCompute&& other) noexcept {
	release();
	m_id = std::exchange(other.m_id, 0);
	m_groupCountX = std::exchange(other.m_groupCountX, 0);
	m_groupCountY = std::exchange(other.m_groupCountY, 0);
	m_groupCountZ = std::exchange(other.m_groupCountZ, 0);
}
GLCompute& GLCompute::operator=(GLCompute&& other) noexcept {
	if (this != &other) {
		release();
		m_id = std::exchange(other.m_id, 0);
		m_groupCountX = std::exchange(other.m_groupCountX, 0);
		m_groupCountY = std::exchange(other.m_groupCountY, 0);
		m_groupCountZ = std::exchange(other.m_groupCountZ, 0);
	}
	return *this;
}

GLuint GLCompute::CompileShader(GLenum shaderType, const char* source) {
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

void GLCompute::release() {
	if (m_id != 0) {
		glDeleteProgram(m_id);
		m_id = 0;
	}
	m_groupCountX = 0;
	m_groupCountY = 0;
	m_groupCountZ = 0;
}