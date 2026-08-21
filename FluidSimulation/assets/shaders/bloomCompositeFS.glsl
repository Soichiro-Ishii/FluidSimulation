#version 460 core

layout(location = 0) in vec2 vUV;
layout(binding = 0) uniform sampler2D uHDR;
layout(binding = 1) uniform sampler2D uBloom;

layout(location = 0) out vec4 outColor;

uniform float uBloomStrength;

void main(){
	vec4 hdr = texture(uHDR,vUV);
	vec4 bloom = texture(uBloom,vUV);
	outColor = hdr + bloom * uBloomStrength;
}
