#version 460 core

layout(location = 0) in vec2 vUV;
layout(binding = 0) uniform sampler2D uTex;

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

uniform uint enableImgInit;

void main(){
    if(enableImgInit == 1){
	    vec4 tex = texture(uTex,vUV);
	    outColDens = tex;
    }
    else{
        outColDens = vec4(0.0,0.0,0.0,0.0);       
    }
}
