#version 460 core

layout(location = 0) in vec2 vUV;
layout(binding = 0) uniform sampler2D uVel;
layout(binding = 1) uniform sampler2D uBeforeColDens;

layout(std140, binding = 0) uniform FluidSimConstants
{
    vec2 uResolution;
    float uTime;
    float uDelta;
	float vortStrength;
	float densityDecay;
	float velocityDecay;
	uint enableSub;

	vec2 uDensityResolution;
	float uPressureRetention;
	float uVelocityDiffusion;

	float uDensityDiffusion;
	float uCellSize;
	float uCellSizeSq;
};

layout(std140, binding = 1) uniform FluidSimInput
{
    vec2 uMPos;
	vec2 uMVel;

	float uInteractionForce;
	float uInteractionRadius;	//UVでの大きさ
	vec2 uGravity;

	vec2 uOtherForce;
	vec2 uOtherAcc;

	vec4 uInjectColor;
	
	vec4 uInjectOutColor;

	uint uRClick;
	uint uLClick;
};

layout(location = 0) out vec4 outColDens;

void main(){
    vec2 vel = texture(uVel,vUV).rg;
    vec2 oldUV = vUV - vel * uDelta / uResolution;
    vec4 newColDens = texture(uBeforeColDens,oldUV);
    outColDens = newColDens;
}
