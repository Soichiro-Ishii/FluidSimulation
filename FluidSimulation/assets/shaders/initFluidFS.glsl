#version 460 core

layout(location = 0) in vec2 vUV;
layout(binding = 0) uniform sampler2D uTex;

layout(std140, binding = 0) uniform FluidSimConstants
{
    vec2 uResolution;
    float uTime;
    float uDelta;
};

layout(location = 0) out vec2 outVel;
layout(location = 1) out vec4 outColDens;

void main(){
	vec4 tex = texture(uTex,vUV);
	vec2 p = vUV * 2.0 - 1.0;
	p.x *= uResolution.x / uResolution.y;
	p.y *= -1;
	outVel = vec2(0.0,0.0);
	outColDens = tex;
}
