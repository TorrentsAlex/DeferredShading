#version 330

const float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216);
in vec2 fragUV;

uniform sampler2D gLuminance;

uniform int blurType;

out vec4 finalColor;

vec4 horitzontalBlur(float offset) {

    vec4 color = vec4(0.0f);
    for(int i = 0; i < 5; i++) {
        color += texture(gLuminance, fragUV + vec2(offset* i, 0.0)) * weight[i];
        color += texture(gLuminance, fragUV - vec2(offset* i, 0.0)) * weight[i];
    }
    return color;
}

vec4 verticalBlur(float offset) {
  vec4 color = vec4(0.0f);
  for(int i = 0; i < 5; i++) {
    color += texture(gLuminance, fragUV + vec2(0.0, offset* i)) * weight[i];
    color += texture(gLuminance, fragUV - vec2(0.0, offset* i)) * weight[i];
  }
  return color;
}

void main() {
    finalColor = texture(gLuminance, fragUV);
    vec2 texOffset = 1.0 / textureSize(gLuminance, 0);
    if (blurType == 0) {
        finalColor = horitzontalBlur(texOffset.x);
    } else {
        finalColor = verticalBlur(texOffset.y);
    }
}
