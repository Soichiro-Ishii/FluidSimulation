#version 460 core

layout(location = 0) in vec2 vUV;
layout(binding = 0) uniform sampler2D uTex;

layout(location = 0) out vec4 outColor;

uniform uint uProcCount;
uniform float uBlurScale;
uniform uint uRadius;
uniform float uSigmaExtent;

float gaussian(float x, float sigma)
{
    return exp(-(x * x) / (2.0 * sigma * sigma));
}

void main()
{
    vec2 texelSize = 1.0 / vec2(textureSize(uTex, 0));
    vec2 offset;
    if(uProcCount % 2 == 0)
        offset = vec2(texelSize.x,0.0) * uBlurScale;
    else
        offset = vec2(0.0,texelSize.y) * uBlurScale;
    vec3 color = vec3(0.0);
    float weightSum = 0.0;
    float sigma = uRadius / uSigmaExtent;
    for(int i = -int(uRadius);i<=int(uRadius);i++){
        float w = gaussian(float(i), sigma);
        color += texture(uTex, vUV + offset * float(i)).rgb * w;
        weightSum += w;
    }

    outColor = vec4(color / weightSum, 1.0);
}
