#version 460 core

layout(location = 0) in vec2 vUV;
layout(binding = 0) uniform sampler2D uTex;

layout(location = 0) out vec4 outColor;

uniform uint uProcCount;
uniform float uBlurScale;

void main()
{
    vec2 texelSize = 1.0 / vec2(textureSize(uTex, 0));

    float weights[5] = float[](
        0.227027,
        0.1945946,
        0.1216216,
        0.054054,
        0.016216
    );


    vec3 color = texture(uTex,vUV).rgb * weights[0];
    vec2 offset;
    if(uProcCount % 2 == 0)
        offset = vec2(texelSize.x,0.0) * uBlurScale;
    else
        offset = vec2(0.0,texelSize.y) * uBlurScale;

    for(int i = 1;i<5;i++){
        color += texture(uTex,vUV + offset * i).rgb * weights[i];
        color += texture(uTex,vUV - offset * i).rgb * weights[i];
    }

    outColor = vec4(color, 1.0);
}