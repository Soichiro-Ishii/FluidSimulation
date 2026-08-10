#version 460 core

layout(location = 0) in vec2 vUV;
layout(binding = 0) uniform sampler2D uVel;

layout(std140, binding = 0) uniform FluidSimConstants
{
    vec2 uResolution;
    float uTime;
    float uDelta;
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

layout(location = 0) out vec2 outVel;

void main(){
	vec2 mousePosUV = uMPos / uResolution; 
	mousePosUV.y = 1.0 - mousePosUV.y;
	vec2 mouseDeltaUV = uMVel / uResolution;
	mouseDeltaUV.y *= -1.0;
	vec2 dirMouse = mousePosUV - vUV;
	vec2 vel = texture(uVel,vUV).xy;
	float mouseSpeed = length(mouseDeltaUV);
if(length(dirMouse) < uInteractionRadius && uRClick == 1 && mouseSpeed > 1e-6)
	outVel = mouseDeltaUV * uInteractionForce * uDelta + vel;
else
	outVel = vel;
}
