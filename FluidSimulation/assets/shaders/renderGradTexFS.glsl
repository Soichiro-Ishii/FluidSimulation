#version 460 core

layout(location = 0) in vec2 vUV;
layout(binding = 0) uniform sampler2D uTex;

layout(location = 0) out vec4 outColor;

void main(){
	vec4 texR = textureOffset(uTex,vUV,ivec2(1,0),0);
	vec4 texL = textureOffset(uTex,vUV,ivec2(-1,0),0);
	vec4 texU = textureOffset(uTex,vUV,ivec2(0,1),0);
	vec4 texB = textureOffset(uTex,vUV,ivec2(0,-1),0);
	vec4 gradX = texR - texR;
	vec4 gradY = texU - texB;
	vec4 grad = gradX * gradX + gradY * gradY;
	outColor = vec4(sqrt(grad.r),sqrt(grad.g),sqrt(grad.b),sqrt(grad.a));
}
