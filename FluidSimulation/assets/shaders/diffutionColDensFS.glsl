#version 460 core

layout(location = 0) in vec2 vUV;
layout(binding = 0) uniform sampler2D uBeforeColDens;
layout(binding = 1) uniform sampler2D uGuess;

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

layout(location = 0) out vec4 outQ;

void main(){
	vec4 old = textureOffset(uBeforeColDens,vUV,ivec2(0,0));

	vec4 L = textureOffset(uGuess,vUV,ivec2(-1,0),0);
	vec4 R = textureOffset(uGuess,vUV,ivec2(1,0),0);
	vec4 B = textureOffset(uGuess,vUV,ivec2(0,-1),0);
	vec4 T = textureOffset(uGuess,vUV,ivec2(0,1),0);

	float a = uDensityDiffusion * uDelta / uCellSizeSq;

	outQ = (old + a * (L + R + B + T)) / (1.0 + 4.0 * a);
}
