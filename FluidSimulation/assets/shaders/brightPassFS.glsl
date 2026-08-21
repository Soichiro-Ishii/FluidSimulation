#version 460 core

layout(location = 0) in vec2 vUV;
layout(binding = 0) uniform sampler2D uTex;

layout(location = 0) out vec4 outColor;

uniform float uThreshold;

void main(){
	vec3 tex = texture(uTex,vUV).rgb;
	float brightness = dot(tex, vec3(0.2126, 0.7152, 0.0722));
	if(brightness > uThreshold)
		outColor = vec4(tex,1.0);
	else	
		outColor = vec4(0.0);
}
