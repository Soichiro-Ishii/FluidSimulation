#version 460 core

layout(location = 0) in vec2 vUV;
layout(binding = 0) uniform sampler2D uDivergence;
layout(binding = 1) uniform sampler2D uBeforePressure;

layout(std140, binding = 0) uniform FluidSimConstants
{
    vec2 uResolution;
    float uTime;
    float uDelta;
};

layout(location = 0) out float outPressure;

void main(){
    float pR = textureOffset(uBeforePressure,vUV,ivec2(1,0)).x;
    float pL = textureOffset(uBeforePressure,vUV,ivec2(-1,0)).x;

    float pT = textureOffset(uBeforePressure,vUV,ivec2(0,1)).x;
    float pB = textureOffset(uBeforePressure,vUV,ivec2(0,-1)).x;

    float d = texture(uDivergence,vUV).r;

    outPressure = (pR + pL + pT + pB - d) * 0.25;
}
