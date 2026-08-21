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
	uint32_t enableSub;

	glm::vec2 densityResolution;
	float pressureRetention;
	float velocityDiffusion;

	float densityDiffusion;
	float cellSize;
	float cellSizeSq;
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

	glm::vec4 injectOutColor;

	uint32_t rClick;
	uint32_t lClick;

};

struct alignas(16) RenderState {
	float baseStrength;
	float gradStrength;
	float interactionStrength;
	uint32_t viewMode;
	float viewStrength;
};

enum class COLOR_CONTROL_MODE {
	RAINBOW,
	OWN
};

enum class FLUID_VIEW_MODE {
	DENSITY,
	VELOCITY,
	PRESSURE,
	DIVERGENCE,
	CURL
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
	GLRenderTarget m_diffVelGuessRT[2];
	GLRenderTarget m_diffColDensGuessRT[2];
	GLRenderTarget m_renderFluidRT;
	std::vector<GLRenderTarget*> m_notDensRenderTargets;
	GLShader m_advVelShader;
	GLShader m_advColDensShader;
	GLShader m_renderTexShader;
	GLShader m_renderGradTexShader;
	GLShader m_renderGradTexBlendShader;
	GLShader m_initColDensShader;
	GLShader m_initVelShader;
	GLShader m_divergenceShader;
	GLShader m_renderDivShader;
	GLShader m_initPressureShader;
	GLShader m_jacobiPressureShader;
	GLShader m_jacobiDiffVelShader;
	GLShader m_jacobiDiffColDensShader;
	GLShader m_projectionShader;
	GLShader m_applyForceVelShader;
	GLShader m_applyInputColDensShader;
	GLShader m_vortOmegaShader;
	GLShader m_renderFluidShader;
	GLMesh m_screen;
	GLUniformBuffer m_FSCUB;
	GLUniformBuffer m_MSUB;
	GLUniformBuffer m_RSUB;
	FluidSimConstants m_FSC;
	FluidSimInput m_MS;
	RenderState m_RS;
	char m_currentColDensRT = 0;
	GLTexture2D m_firstColDens;
	bool m_shouldReset = true;
	GLSampler m_texcelSMP;
	unsigned int m_numJacobiReps = 32;
	unsigned int m_numDiffutionJacobiReps = 32;
	char m_currentVelRT = 0;
	bool m_enableImgInit = true;
	float m_phaseShift = 0.5f;
	float m_colChangeSpeed = 1.0f / 5.0f;
	float m_inSaturation = 1.0f;
	float m_inBrightness = 1.0f;
	float m_outSaturation = 1.0f;
	float m_outBrightness = 1.0f;
	COLOR_CONTROL_MODE m_inColCtrMode = COLOR_CONTROL_MODE::RAINBOW;
	COLOR_CONTROL_MODE m_outColCtrMode = COLOR_CONTROL_MODE::RAINBOW;
	float m_colorStrength = 200.0f;
	bool m_colInOutLock = false;
	FLUID_VIEW_MODE m_fluidViewMode = FLUID_VIEW_MODE::DENSITY;
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

