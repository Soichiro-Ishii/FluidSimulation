#version 460 core

layout(location = 0) in vec2 vUV;
layout(binding = 0) uniform sampler2D uVel;

layout(std140, binding = 0) uniform FluidSimConstants
{
    vec2 uResolution;
    float uTime;
    float uDelta;
};

layout(location = 0) out float outDivergence;

void main(){
    float uR = textureOffset(uVel,vUV,ivec2(1,0)).x;
    float uL = textureOffset(uVel,vUV,ivec2(-1,0)).x;

    float vT = textureOffset(uVel,vUV,ivec2(0,1)).y;
    float vB = textureOffset(uVel,vUV,ivec2(0,-1)).y;

    outDivergence = 0.5 * (uR - uL + vT - vB);
}
