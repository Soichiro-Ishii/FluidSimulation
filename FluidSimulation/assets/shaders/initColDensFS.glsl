#version 460 core

layout(location = 0) in vec2 vUV;
layout(binding = 0) uniform sampler2D uTex;

layout(std140, binding = 0) uniform FluidSimConstants
{
    vec2 uResolution;
    float uTime;
    float uDelta;
};

layout(location = 0) out vec4 outColDens;

uniform uint enableImgInit;

void main(){
    if(enableImgInit == 1){
	    vec4 tex = texture(uTex,vUV);
	    outColDens = tex;
    }
    else{
        outColDens = vec4(0.0,0.0,0.0,0.0);       
    }
}
