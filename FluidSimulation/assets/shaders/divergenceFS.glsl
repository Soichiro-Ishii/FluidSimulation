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

	uint uRClick;
	uint uLClick;
};

layout(location = 0) out float outDivergence;

void main(){
    float uR = textureOffset(uVel,vUV,ivec2(1,0)).x;
    float uL = textureOffset(uVel,vUV,ivec2(-1,0)).x;

    float vT = textureOffset(uVel,vUV,ivec2(0,1)).y;
    float vB = textureOffset(uVel,vUV,ivec2(0,-1)).y;

    outDivergence = 0.5 * (uR - uL + vT - vB);
}
