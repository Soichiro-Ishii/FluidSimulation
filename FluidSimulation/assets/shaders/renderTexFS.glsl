#version 460 core

layout(location = 0) in vec2 vUV;
layout(binding = 0) uniform sampler2D uTex;

layout(location = 0) out vec4 outColor;

void main(){
	vec4 tex = texture(uTex,vUV);
	outColor = tex;
}
