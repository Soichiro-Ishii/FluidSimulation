#pragma once
#include"TextureSmpSettings.h"

struct SAMPLER_DESC {
	TEXTURE2DFILTER minFilter = TEXTURE2DFILTER::LINEAR;
	TEXTURE2DFILTER magFilter = TEXTURE2DFILTER::LINEAR;

	TEXTURE2DWRAP wrapS = TEXTURE2DWRAP::CLAMP_TO_EDGE;
	TEXTURE2DWRAP wrapT = TEXTURE2DWRAP::CLAMP_TO_EDGE;
	TEXTURE2DWRAP wrapR = TEXTURE2DWRAP::CLAMP_TO_EDGE;
};

class GLSampler
{
private:
	GLuint m_id;
	SAMPLER_DESC m_desc;
public:
	GLSampler() = default;
	~GLSampler();
	GLSampler(SAMPLER_DESC _desc);

	GLSampler(GLSampler&) = delete;
	GLSampler& operator=(GLSampler&) = delete;

	GLSampler(GLSampler&& other) noexcept;
	GLSampler& operator=(GLSampler&& other) noexcept;

	void create(SAMPLER_DESC _desc);
	void bind(GLuint _id);
	void unbind(GLuint _id);

	[[nodiscard]] GLuint id() {
		return m_id;
	}
	[[nodiscard]] bool vaild() {
		return m_id != 0;
	}
	[[nodiscard]] SAMPLER_DESC desc() {
		return m_desc;
	}

	void release();
};

