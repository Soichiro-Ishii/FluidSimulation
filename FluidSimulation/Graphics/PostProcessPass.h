#pragma once
#include"GLShader.h"
#include"GLMesh.h"
#include"GLRenderTarget.h"
#include"GLTexture2D.h"
class PostProcessPass
{
private:
	const GLShader* m_shader = nullptr;
	GLRenderTarget m_rt;
public:
	PostProcessPass() = default;
	PostProcessPass(GLShader& shader, int width, int height, ColorTexSet cSet);
	~PostProcessPass() = default;

	PostProcessPass(PostProcessPass&) = delete;
	PostProcessPass& operator=(PostProcessPass&) = delete;

	PostProcessPass(PostProcessPass&& other) noexcept;
	PostProcessPass& operator=(PostProcessPass&& other) noexcept;

	bool create(GLShader& shader, int width, int height, ColorTexSet cSet);
	void resize(int width, int height);
	void execute(const GLTexture2D& texture, const GLMesh& screen);

	[[nodiscard]] const GLTexture2D& output() const {
		return m_rt.color();
	}
	[[nodiscard]] const GLShader& shader() const {
		return *m_shader;
	}
};

