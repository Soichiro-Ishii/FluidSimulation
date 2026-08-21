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
	vec4 texR = textureOffset(uTex,vUV,ivec2(1,0),0);
	vec4 texL = textureOffset(uTex,vUV,ivec2(-1,0),0);
	vec4 texU = textureOffset(uTex,vUV,ivec2(0,1),0);
	vec4 texB = textureOffset(uTex,vUV,ivec2(0,-1),0);
	vec4 gradX = texR - texL;
	vec4 gradY = texU - texB;
	vec4 grad = gradX * gradX + gradY * gradY;

	vec4 tex = texture(uTex,vUV);
	vec4 texGrad = vec4(sqrt(grad.r),sqrt(grad.g),sqrt(grad.b),sqrt(grad.a));

	outColor =
		tex * uBaseStrength +
		texGrad * uGradStrength +
		tex * texGrad * uInteractionStrength;
}
