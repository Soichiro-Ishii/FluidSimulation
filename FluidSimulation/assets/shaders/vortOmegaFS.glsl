#version 460 core

layout(location = 0) in vec2 vUV;
layout(binding = 0) uniform sampler2D uVel;

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

layout(location = 0) out float outVortOmega;

void main(){
    float uT = textureOffset(uVel,vUV,ivec2(0,1)).x;
    float uB = textureOffset(uVel,vUV,ivec2(0,-1)).x;

    float vR = textureOffset(uVel,vUV,ivec2(1,0)).y;
    float vL = textureOffset(uVel,vUV,ivec2(-1,0)).y;

    outVortOmega = 0.5 * ((vR - vL) - (uT - uB));
}
