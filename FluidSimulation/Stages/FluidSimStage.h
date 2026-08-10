#pragma once
#include "Stage.h"
#include "GLShader.h"
#include "GLTexture2D.h"
#include "GLUniformBuffer.h"
#include"GLRenderTarget.h"
#include"GLMultiRenderTarget.h"
#include "GLMesh.h"

struct alignas(16) FluidSimConstants
{
	glm::vec2 resolution;
	float time;
	float delta;
};

class FluidSimStage :
	public Stage
{
private:
	GLMultiRenderTarget m_rt[2];
	GLShader m_advShader;
	GLShader m_lastShader;
	GLShader m_initFluidShader;
	GLMesh m_screen;
	GLUniformBuffer m_FSCUB;
	FluidSimConstants m_FSC;
	char m_currentRT = 0;
	GLTexture2D m_firstColDens;
	bool firstFrame = true;
public:
	FluidSimStage();
	bool onInit() override;
	void onUpdate(float delta) override;
	void onRender() override;
	void onShutdown() override;
private:
	void setStageName() override;
};

