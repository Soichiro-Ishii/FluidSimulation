#version 460 core

layout(location = 0) in vec2 vUV;
layout(binding = 0) uniform sampler2D uVel;
layout(binding = 1) uniform sampler2D uVortOmega;

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

layout(location = 0) out vec2 outVel;

void main(){
	vec2 F = vec2(0.0,0.0);
	vec2 mousePosUV = uMPos / uResolution; 
	mousePosUV.y = 1.0 - mousePosUV.y;
	vec2 mouseDeltaUV = uMVel / uResolution;
	mouseDeltaUV.y *= -1.0;
	vec2 dirMouse = mousePosUV - vUV;
	vec2 vel = texture(uVel,vUV).xy;
	float mouseSpeed = length(mouseDeltaUV);
	float d = length(dirMouse);
	if((uRClick == 1 || uLClick == 1) && mouseSpeed > 1e-6){
		float d2 = dot(dirMouse, dirMouse);

		float sigma = uInteractionRadius * 0.4;

		float weight = exp(
			-d2 / (2.0 * sigma * sigma)
		);
		F = mouseDeltaUV * uInteractionForce * weight;
	}

	float wR = abs(textureOffset(uVortOmega,vUV,ivec2(1,0),0).r);
	float wL = abs(textureOffset(uVortOmega,vUV,ivec2(-1,0),0).r);
	float wT= abs(textureOffset(uVortOmega,vUV,ivec2(0,1),0).r);
	float wB = abs(textureOffset(uVortOmega,vUV,ivec2(0,-1),0).r);

	vec2 gradAbsW = 0.5 * vec2(wR - wL , wT - wB);
	vec2 N = gradAbsW / (length(gradAbsW) + 1e-6);
	float w = texture(uVortOmega,vUV).r;
	F += vortStrength * w * vec2(N.y,-N.x);

	vec2 p = vUV * 2.0 - 1.0;
	outVel = vel + F * uDelta;
	outVel *= exp(-velocityDecay * uDelta);
}
