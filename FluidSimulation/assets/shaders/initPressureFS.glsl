#version 460 core

layout(location = 0) in vec2 vUV;

layout(std140, binding = 0) uniform FluidSimConstants
{
    vec2 uResolution;
    float uTime;
    float uDelta;
};

layout(location = 0) out float outPressure;

void main(){
	outPressure = 0.0;
}
