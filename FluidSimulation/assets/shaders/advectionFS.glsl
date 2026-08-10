#version 460 core

layout(location = 0) in vec2 vUV;
layout(binding = 0) uniform sampler2D uBeforeVel;
layout(binding = 1) uniform sampler2D uBeforeColDens;

layout(std140, binding = 0) uniform FluidSimConstants
{
    vec2 uResolution;
    float uTime;
    float uDelta;
};

layout(location = 0) out vec2 outVel;
layout(location = 1) out vec4 outColDens;

void main(){
    vec2 vel = texture(uBeforeVel,vUV).rg;
    vec2 oldUV = vUV - vel * uDelta / uResolution;
    vec2 newVel = texture(uBeforeVel,oldUV).rg;
    vec4 newColDens = texture(uBeforeColDens,oldUV);

    vec2 p = vUV * 2.0 - 1.0;
	p.x *= uResolution.x / uResolution.y;
	p.y *= -1;

    newVel += vec2(p.y,p.x) * 10 * (1 / length(p)) * uDelta;

    outVel = newVel;
    outColDens = newColDens;
}
