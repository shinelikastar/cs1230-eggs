# version 400 core

in vec2 uv;

uniform sampler2D tex;
uniform sampler2D depthVals;
uniform float fStop;
uniform float planeInFocus;

layout(location=0) out vec4 fragColor;
layout(location=1) out vec4 depth;

void horizontalBlur(int supportWidth, vec2 texelSize) {
    fragColor = vec4(0.0);
    float weights = 0.0;
    for (int i = -supportWidth; i <= supportWidth; i++) {
        float weight = (supportWidth + 1) - abs(i);
        vec2 displace = vec2(i * texelSize.x, 0);
        fragColor += weight * texture(tex, uv + displace);
        weights += weight;
    }
    fragColor /= weights;
}

void main(){
    fragColor = vec4(1.0);

    vec2 texelSize = 1.0 / textureSize(tex, 0).xy;

    float curDepth = texture(depthVals, uv).r;
    int supportWidth = int(abs((1 - planeInFocus/55.) - curDepth) * (20.0 / fStop));

    supportWidth = min(supportWidth, 100);

    horizontalBlur(supportWidth, texelSize);

    depth = texture(depthVals, uv);
}

