#version 330

in vec2 fragUV;
in vec3 fragNormal;
in vec3 fragPosition;

uniform vec2 ab;
uniform vec3 viewerPosition;
uniform mat4 projectionMatrix;

out vec4 gDiffuse;
out vec4 gNormal;
out vec4 gPosition;

vec2 sphereMap(vec3 nor) {
	return normalize(nor.xy) * sqrt(-nor.z*0.5 + 0.5) * 0.5 + 0.5;
}

void main() {

	float depth = ab.x + ab.y / -length(viewerPosition - fragPosition);

	gDiffuse = vec4(fragUV, 0.0, 1.0);
	gNormal = vec4(sphereMap(fragNormal), depth, 1.0);
	gPosition = vec4(fragPosition, 1.0);
}
