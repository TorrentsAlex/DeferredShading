#version 330

in vec2 fragUV;

uniform sampler2D gColor;
uniform sampler2D gBloom;

out vec4 fragColor;

#define SHOULDER_STRENGTH 0.22
#define LINEAR_STRENGTH 0.30
#define LINEAR_ANGLE 0.10
#define TOE_STRENGTH 0.20
#define TOE_NUMERATOR 0.01
#define TOE_DENOMINATOR 0.30
#define LINEAR_WHITE_POINT_VALUE 11.2

vec4 filmic_tonemapping(vec4 x)
{
    return ((x*(SHOULDER_STRENGTH*x + LINEAR_ANGLE * LINEAR_STRENGTH) + TOE_STRENGTH * TOE_NUMERATOR) /
            (x*(SHOULDER_STRENGTH*x + LINEAR_STRENGTH) + TOE_STRENGTH * TOE_DENOMINATOR)) - TOE_NUMERATOR/TOE_DENOMINATOR;
}
void main() {


  fragColor = texture(gColor, fragUV) + texture(gBloom, fragUV);

  fragColor = filmic_tonemapping(fragColor) / filmic_tonemapping(vec4(LINEAR_WHITE_POINT_VALUE));
}
