#version 460 core

layout(location = 0) in vec2 vUV;
layout(binding = 0) uniform sampler2D uVel;
layout(binding = 1) uniform sampler2D uBeforeColDens;

layout(std140, binding = 0) uniform FluidSimConstants
{
    vec2 uResolution;
    float uTime;
    float uDelta;
};

layout(location = 0) out vec4 outColDens;

void main(){
    vec2 vel = texture(uVel,vUV).rg;
    vec2 oldUV = vUV - vel * uDelta / uResolution;
    vec4 newColDens = texture(uBeforeColDens,oldUV);
    outColDens = newColDens;
}
