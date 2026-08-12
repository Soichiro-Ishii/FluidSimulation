#version 460 core

layout(location = 0) in vec2 vUV;
layout(binding = 0) uniform sampler2D uColDens;

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

layout(location = 0) out vec4 outColDens;

void main(){
	vec2 mousePosUV = uMPos / uResolution; 
	mousePosUV.y = 1.0 - mousePosUV.y;
	vec2 dirMouse = mousePosUV - vUV;
	vec4 colDens = texture(uColDens,vUV);
	float d = length(dirMouse);
	float speed = length(uMVel);

if(uLClick == 1 && speed > 0){
	float d2 = dot(dirMouse, dirMouse);

	float sigma = uInteractionRadius * 0.4;

	float weight = exp(
		-d2 / (2.0 * sigma * sigma)
	);
	float t = 1.0 - (d / uInteractionRadius);
	outColDens = uInjectColor * weight * uDelta + colDens;
}
else
	outColDens = colDens;

	outColDens *= exp(-densityDecay * uDelta);
}
