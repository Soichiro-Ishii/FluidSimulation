#version 460 core

layout(location = 0) in vec2 vUV;

layout(std140, binding = 0) uniform FluidSimConstants
{
    vec2 uResolution;
    float uTime;
    float uDelta;
};

layout(location = 0) out vec2 outVel;

void main(){
	outVel = vec2(0.0,0.0);
}
