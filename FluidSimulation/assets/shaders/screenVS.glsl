#version 460 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec2 aUV;

layout(location = 0) out vec2 vUV;

void main(){
	gl_Position = vec4(aPosition,1.0);
	vUV = aUV;
}