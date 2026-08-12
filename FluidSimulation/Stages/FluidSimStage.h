#pragma once
#include "Stage.h"
#include "GLShader.h"
#include "GLTexture2D.h"
#include "GLUniformBuffer.h"
#include"GLRenderTarget.h"
#include"GLMultiRenderTarget.h"
#include "GLMesh.h"
#include"GLSampler.h"

struct alignas(16) FluidSimConstants
{
	glm::vec2 resolution;
	float time;
	float delta;
	float vortStrength;
	float densityDecay;
	float velocityDecay;
};

struct alignas(16) FluidSimInput {
	glm::vec2 mPos;
	glm::vec2 mVel;

	float interactionForce;
	float interactionRadius;	//UVでの大きさ
	glm::vec2 gravity;

	glm::vec2 otherForce;
	glm::vec2 otherAcc;

	glm::vec4 injectColor;

	uint32_t rClick;
	uint32_t lClick;
};

class FluidSimStage :
	public Stage
{
private:
	GLRenderTarget m_velRT[2];		//0番は移流 1番はprojection
	GLRenderTarget m_colDensRT[2];
	GLRenderTarget m_divergenceRT;
	GLRenderTarget m_pressureRT[2];
	GLRenderTarget m_vortOmegaRT;
	std::vector<GLRenderTarget*> m_renderTargets;
	GLShader m_advVelShader;
	GLShader m_advColDensShader;
	GLShader m_renderTexShader;
	GLShader m_renderGradTexShader;
	GLShader m_initColDensShader;
	GLShader m_initVelShader;
	GLShader m_divergenceShader;
	GLShader m_renderDivShader;
	GLShader m_initPressureShader;
	GLShader m_jacobiPressureShader;
	GLShader m_projectionShader;
	GLShader m_applyForceVelShader;
	GLShader m_applyInputColDensShader;
	GLShader m_vortOmegaShader;
	GLMesh m_screen;
	GLUniformBuffer m_FSCUB;
	GLUniformBuffer m_MSUB;
	FluidSimConstants m_FSC;
	FluidSimInput m_MS;
	char m_currentColDensRT = 0;
	GLTexture2D m_firstColDens;
	bool m_shouldReset = true;
	GLSampler m_texcelSMP;
	unsigned int m_numJacobiReps = 32;
	char m_currentVelRT = 0;
	bool m_showGrad = false;
	float m_saturation = 1.0f;
	float m_brightness = 1.0f;
	bool m_enableImgInit = true;
public:
	FluidSimStage();
	bool onInit() override;
	void onUpdate(float delta) override;
	void onRender() override;
	void onShutdown() override;
private:
	void setStageName() override;
	void changeRTResolution(int newWidth, int newHeight);
};

