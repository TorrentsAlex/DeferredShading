#version 330

in vec2 fragUV;
in vec3 fragNormal;
in vec3 fragPosition;

uniform sampler2D textureData;
uniform sampler2D specularMap;

out vec4 gDiffuse;
out vec4 gNormal;
out vec4 gPosition;

void main() {
	vec4 spec = texture(specularMap, fragUV);

	gDiffuse = vec4(texture(textureData, fragUV).rgb, 0.5);
	gNormal = vec4(fragNormal, 0.5);
	gPosition = vec4(fragPosition, 1.0);
}
