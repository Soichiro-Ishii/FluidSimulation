#version 460 core

layout(location = 0) in vec2 vUV;
layout(binding = 0) uniform sampler2D uTex;

layout(std140, binding = 2) uniform RenderState
{
	float uBaseStrength;
	float uGradStrength;
	float uInteractionStrength;
	uint uViewMode;
	float uViewStrength;
};

layout(location = 0) out vec4 outColor;

void main(){
	vec4 tex = texture(uTex,vUV);
	outColor = tex * uBaseStrength;
}
