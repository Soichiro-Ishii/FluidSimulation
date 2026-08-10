#include "pch.h"
#include "GLTexture2D.h"
#define STB_IMAGE_IMPLEMENTATION
#include<stb_image.h>
#include"path.h"

GLTexture2D::GLTexture2D(std::string texPath, TEXTURE2DSETTING set) {
	load(texPath, set);
}

GLTexture2D::GLTexture2D(TEXTURE2DDESC& desc) {
	create(desc);
}

GLTexture2D::~GLTexture2D() {
	release();
}

GLTexture2D::GLTexture2D(GLTexture2D&& other) noexcept {
	m_id = std::exchange(other.m_id, 0);
	m_desc = std::exchange(other.m_desc, TEXTURE2DDESC{});
}
GLTexture2D& GLTexture2D::operator=(GLTexture2D&& other) noexcept {
	if (this != &other) {
		release();
		m_id = std::exchange(other.m_id, 0);
		m_desc = std::exchange(other.m_desc, TEXTURE2DDESC{});
	}
	return *this;
}

bool GLTexture2D::loadHDR(std::string& texPath, TEXTURE2DDESC& desc) {
	int originalChannels = 0;
	stbi_set_flip_vertically_on_load(true);
	float* data = stbi_loadf(
		texPath.c_str(),
		&desc.width,
		&desc.height,
		&originalChannels,
		4
	);

	if (!data) {
		spdlog::error(
			"Failed to load texture\n"
			"Reason: {}\n"
			"Relative path: {}\n"
			"Absolute path: {}",
			stbi_failure_reason(),
			texPath,
			std::filesystem::absolute(texPath).string()
		);

		return false;
	}
	spdlog::info(
		"Texture loaded: {} ({}x{}, channels={})",
		texPath,
		desc.width,
		desc.height,
		originalChannels
	);

	create(desc, data);

	stbi_image_free(data);

	return true;
}
bool GLTexture2D::loadNORMAL(std::string& texPath, TEXTURE2DDESC& desc) {
	int originalChannels = 0;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(
		texPath.c_str(),
		&desc.width,
		&desc.height,
		&originalChannels,
		4
	);

	if (!data) {
		spdlog::error(
			"Failed to load texture\n"
			"Reason: {}\n"
			"Relative path: {}\n"
			"Absolute path: {}",
			stbi_failure_reason(),
			texPath,
			std::filesystem::absolute(texPath).string()
		);

		return false;
	}
	spdlog::info(
		"Texture loaded: {} ({}x{}, channels={})",
		texPath,
		desc.width,
		desc.height,
		originalChannels
	);

	create(desc, data);

	stbi_image_free(data);

	return true;
}

bool GLTexture2D::load(std::string& texPath, TEXTURE2DSETTING set) {
	std::string ext = splitFileAndExt(texPath).second;
	TEXTURE2DDESC desc;
	desc.set = set;
	if (ext == "hdr" || ext == "HDR") {
		desc.internalFormat = GL_RGBA16F;	//hdrのときはsRGB関係ないから変えない
		desc.format = GL_RGBA;
		desc.type = GL_FLOAT;
		return loadHDR(texPath, desc);
	}
	//それ以外
	if (set.colorSpace == COLOR_SPACE::RGB)
		desc.internalFormat = GL_RGBA8;
	else
		desc.internalFormat = GL_SRGB8_ALPHA8;
	desc.format = GL_RGBA;
	desc.type = GL_UNSIGNED_BYTE;
	return loadNORMAL(texPath, desc);
}

void GLTexture2D::create(TEXTURE2DDESC& desc, void* data) {
	release();
	m_desc = desc;
	//テクスチャ作成
	glGenTextures(1, &m_id);
	//バインド
	glBindTexture(GL_TEXTURE_2D, m_id);
	//書き込み
	glTexImage2D(
		GL_TEXTURE_2D,	//2D
		0,				//ミニマップレベル
		m_desc.internalFormat,		//形式
		m_desc.width,		//幅
		m_desc.height,		//高さ
		0,				//常に0
		m_desc.format,		//並び
		m_desc.type,//型
		data
	);
	//フィルター設定
	GLuint filter;
	if (m_desc.set.filter == TEXTURE2DFILTER::NEAREST)
		filter = GL_NEAREST;
	else
		filter = GL_LINEAR;
	glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_MIN_FILTER,
		filter
	);

	glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_MAG_FILTER,
		filter
	);
	//uv範囲外設定
	GLuint wrap;
	if (m_desc.set.wrap == TEXTURE2DWRAP::REPEAT)
		wrap = GL_REPEAT;
	else if (m_desc.set.wrap == TEXTURE2DWRAP::CLAMP_TO_EDGE)
		wrap = GL_CLAMP_TO_EDGE;
	else
		wrap = GL_MIRRORED_REPEAT;
	glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_S,
		wrap
	);
	glTexParameteri(
		GL_TEXTURE_2D,
		GL_TEXTURE_WRAP_T,
		wrap
	);
	//バインド解除
	glBindTexture(GL_TEXTURE_2D, 0);

	spdlog::info(
		"Texture created: {}x{}",
		m_desc.width,
		m_desc.height
	);
}

void GLTexture2D::bind(GLuint textureId) const {
	glActiveTexture(GL_TEXTURE0 + textureId);
	glBindTexture(GL_TEXTURE_2D, m_id);
}
void GLTexture2D::unBind(GLuint textureId) const {
	glActiveTexture(GL_TEXTURE0 + textureId);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GLTexture2D::resize(int newWidth, int newHeight) {
	if (newWidth == m_desc.width && newHeight == m_desc.height)
		return;
	if (newWidth <= 0 || newHeight <= 0)
		return;
	if (m_id == 0)
		return;

	glBindTexture(GL_TEXTURE_2D, m_id);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		m_desc.internalFormat,
		newWidth,
		newHeight,
		0,
		m_desc.format,
		m_desc.type,
		nullptr
	);
	m_desc.width = newWidth;
	m_desc.height = newHeight;
}

void GLTexture2D::release() {
	if (m_id != 0) {
		glDeleteTextures(1, &m_id);
		m_id = 0;
	}
	m_desc = TEXTURE2DDESC{};
}