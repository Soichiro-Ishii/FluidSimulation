#version 460 core

layout(location = 0) in vec2 vUV;
layout(binding = 0) uniform sampler2D uVel;
layout(binding = 1) uniform sampler2D uPressure;

layout(std140, binding = 0) uniform FluidSimConstants
{
    vec2 uResolution;
    float uTime;
    float uDelta;
};

layout(location = 0) out vec2 outVel;

void main(){
    float pR = textureOffset(uPressure,vUV,ivec2(1,0)).x;
    float pL = textureOffset(uPressure,vUV,ivec2(-1,0)).x;
    float pT = textureOffset(uPressure,vUV,ivec2(0, 1)).x;
    float pB = textureOffset(uPressure,vUV,ivec2(0,-1)).x;

    vec2 gradP = 0.5 * vec2(pR - pL,pT - pB);
    outVel = texture(uVel, vUV).xy - gradP;
}
