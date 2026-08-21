#version 460 core

layout(location = 0) in vec2 vUV;
layout(binding = 0) uniform sampler2D uColDens;
layout(binding = 1) uniform sampler2D uVel;
layout(binding = 2) uniform sampler2D uPres;
layout(binding = 3) uniform sampler2D uDiv;
layout(binding = 4) uniform sampler2D uCurl;


layout(std140, binding = 2) uniform RenderState
{
	float uBaseStrength;
	float uGradStrength;
	float uInteractionStrength;
	uint uViewMode;
	float uViewStrength;
};

layout(location = 0) out vec4 outColor;

#define PI 3.141592653589793

vec3 hsv2rgb_smooth(vec3 c) {
    vec3 rgb = clamp(abs(mod(c.x * 6.0 + vec3(0.0, 4.0, 2.0), 6.0) - 3.0) - 1.0, 0.0, 1.0);
    rgb = rgb * rgb * (3.0 - 2.0 * rgb); // Cubic smoothing
    return c.z * mix(vec3(1.0), rgb, c.y);
}

vec3 signedColor(float x){
	return vec3(max(x,0.0),0.0,max(-x,0.0));
}

void main(){
	vec4 tex = vec4(0.0);
	if(uViewMode == 0){
		tex = texture(uColDens,vUV);
	}
	else if(uViewMode == 1){
		vec2 vel = texture(uVel,vUV).xy;
		float speed = length(vel);
		float angle = atan(vel.y,vel.x);
		float hue = angle / (2.0 * PI) + 0.5;
		vec3 rgb = hsv2rgb_smooth(vec3(hue,1.0,speed));
		tex = vec4(rgb,1.0);
	}
	else if(uViewMode == 2){
		float pres = texture(uPres,vUV).r;
		tex = vec4(signedColor(pres),1.0);
	}
	else if(uViewMode == 3){
		float div = texture(uDiv,vUV).r;
		tex = vec4(signedColor(div),1.0);
	}
	else {
		float curl = texture(uCurl,vUV).r;
		tex = vec4(signedColor(curl),1.0);
	}
	outColor = tex * uViewStrength;
}
