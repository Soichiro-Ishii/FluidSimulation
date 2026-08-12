#version 460 core

layout(location = 0) in vec2 vUV;
layout(binding = 0) uniform sampler2D uTex;

layout(location = 0) out vec4 outColor;

void main(){
	vec4 tex = texture(uTex,vUV);
	vec3 d = tex.r > 0 ? vec3(tex.r,0,0) :  vec3(0,0,-tex.r);
	outColor = vec4(d / 100,0);
}
