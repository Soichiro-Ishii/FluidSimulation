#version 460 core

layout(location = 0) in vec2 vUV;
layout(binding = 0) uniform sampler2D uBeforeVel;

layout(std140, binding = 0) uniform FluidSimConstants
{
    vec2 uResolution;
    float uTime;
    float uDelta;
};

layout(location = 0) out vec2 outVel;

void main(){
    vec2 vel = texture(uBeforeVel,vUV).rg;
    vec2 oldUV = vUV - vel * uDelta / uResolution;
    vec2 newVel = texture(uBeforeVel,oldUV).rg;
    outVel = newVel;
}
