#version 460 core

layout(location = 0) in vec2 vUV;
layout(binding = 0) uniform sampler2D uDivergence;
layout(binding = 1) uniform sampler2D uBeforePressure;

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

layout(location = 0) out float outPressure;

void main(){
    float pR = textureOffset(uBeforePressure,vUV,ivec2(1,0)).x;
    float pL = textureOffset(uBeforePressure,vUV,ivec2(-1,0)).x;

    float pT = textureOffset(uBeforePressure,vUV,ivec2(0,1)).x;
    float pB = textureOffset(uBeforePressure,vUV,ivec2(0,-1)).x;

    float d = texture(uDivergence,vUV).r;

    outPressure = (pR + pL + pT + pB - d) * 0.25;
}
