#version 330

in vec2 fragUV;
in vec3 fragNormal;
in vec3 fragPosition;

uniform sampler2D textureData;
uniform sampler2D specularMap;

uniform int haveSpecularMap;

out vec4 gDiffuse;
out vec4 gNormal;
out vec4 gPosition;
out vec4 gSpecular;

void main() {

	gDiffuse = vec4(texture(textureData, fragUV).rgb, 1.0);
	gNormal = vec4(fragNormal, 1.0);
	gPosition = vec4(fragPosition, 1.0);

	if (haveSpecularMap == 1) {
		gSpecular = vec4(texture(specularMap, fragUV).rgb, 1.0);
	} else {
		gSpecular = vec4(vec3(0.0), 1.0);
	}
}
