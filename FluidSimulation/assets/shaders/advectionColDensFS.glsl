#version 460 core

layout(location = 0) in vec2 vUV;
layout(binding = 0) uniform sampler2D uVel;
layout(binding = 1) uniform sampler2D uBeforeColDens;

layout(std140, binding = 0) uniform FluidSimConstants
{
    vec2 uResolution;
    float uTime;
    float uDelta;
	float vortStrength;
	float densityDecay;
	float velocityDecay;
	uint enableSub;

	vec2 uDensityResolution;
	float uPressureRetention;
	float uVelocityDiffusion;

	float uDensityDiffusion;
	float uCellSize;
	float uCellSizeSq;
};

layout(std140, binding = 1) uniform FluidSimInput
{
    vec2 uMPos;
	vec2 uMVel;

	float uInteractionForce;
	float uInteractionRadius;	//UVでの大きさ
	vec2 uGravity;

	vec2 uOtherForce;
	vec2 uOtherAcc;

	vec4 uInjectColor;
	
	vec4 uInjectOutColor;

	uint uRClick;
	uint uLClick;
};

layout(location = 0) out vec4 outColDens;

vec4 catmullRomWeights(float t)
{
    float t2 = t * t;
    float t3 = t2 * t;

    return vec4(
        -0.5 * t +       t2 - 0.5 * t3,
         1.0       - 2.5 * t2 + 1.5 * t3,
         0.5 * t + 2.0 * t2 - 1.5 * t3,
                    -0.5 * t2 + 0.5 * t3
    );
}

vec2 sampleVelocityBicubic(vec2 uv)
{
    ivec2 textureSizeI = textureSize(uVel, 0);
    vec2 textureSizeF = vec2(textureSizeI);

    // uvをテクセル中心基準の座標に変換
    vec2 p = uv * textureSizeF - 0.5;
    ivec2 base = ivec2(floor(p));
    vec2 f = fract(p);

    vec4 wx = catmullRomWeights(f.x);
    vec4 wy = catmullRomWeights(f.y);

    vec2 result = vec2(0.0);

    for (int y = 0; y < 4; ++y) {
        for (int x = 0; x < 4; ++x) {
            ivec2 coord = base + ivec2(x - 1, y - 1);

            // テクセル中心を正規化UVへ戻す
            vec2 sampleUV =
                (vec2(coord) + 0.5) / textureSizeF;

            result += texture(uVel, sampleUV).rg
                    * wx[x]
                    * wy[y];
        }
    }

    return result;
}

vec2 sampleVelocityGaussian(
    vec2 uv,
    int radius,
    float spanInVelocityTexels)
{
    vec2 velocityTexel =
        1.0 / vec2(textureSize(uVel, 0));

    float radiusF = float(radius);
    float sigma = max(radiusF * 0.65, 0.75);

    vec2 velocitySum = vec2(0.0);
    float weightSum = 0.0;

    // 最大5x5。radiusによって3x3または5x5になる
    for (int y = -2; y <= 2; ++y) {
        for (int x = -2; x <= 2; ++x) {
            if (abs(x) > radius || abs(y) > radius)
                continue;

            vec2 samplePosition = vec2(x, y);

            // サンプル範囲を速度テクセル単位で指定
            vec2 offset =
                samplePosition / radiusF
                * spanInVelocityTexels
                * velocityTexel;

            float distanceSq =
                dot(samplePosition, samplePosition);

            float weight =
                exp(-distanceSq / (2.0 * sigma * sigma));

            velocitySum +=
                texture(uVel, uv + offset).rg * weight;

            weightSum += weight;
        }
    }

    return velocitySum / weightSum;
}

vec2 sampleVelocityAdaptive(vec2 uv)
{
    float ratio = max(
        uDensityResolution.x / uResolution.x,
        uDensityResolution.y / uResolution.y
    );

    // 密度と速度がほぼ同じ
    if (ratio <= 1.25) {
        return texture(uVel, uv).rg;
    }

    // 1920x1080 vs 256x256はここ
    if (ratio <= 12.0) {
        return sampleVelocityBicubic(uv);
    }

    // 中程度に差がある場合：3x3
    if (ratio <= 24.0) {
        return sampleVelocityGaussian(
            uv,
            1,
            0.45
        );
    }

    // 1920x1080 vs 64x64はここ：5x5
    return sampleVelocityGaussian(
        uv,
        2,
        0.70
    );
}

void main(){
     vec2 vel = sampleVelocityAdaptive(vUV);

    vec2 oldUV = vUV - vel * uDelta / (uResolution * uCellSize);

    outColDens = texture(uBeforeColDens, oldUV);
}
