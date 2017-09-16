#version 330

in vec3 vertexPosition;
in vec2 vertexUV;
in vec3 vertexNormal;

out vec2 fragUV;
out vec3 positionVS;

void main() {
	gl_Position = vec4(vertexPosition, 1.0);
	positionVS = vertexPosition;
	fragUV = vertexUV;
}
