#version 330

const int maxLigths = 2;

in vec2 fragUV;

uniform sampler2D gDiff;
uniform sampler2D gNorm;
uniform sampler2D gPos;
uniform vec3 viewerPosition;

out vec4 fragColor;

const float PI = 3.1415926536;

// pass for uniforms
const float roughness = 0.5;
const float metalness = 0.5;

/* Schlick */
vec3 fresnel(float VdotH, vec3 c) {
	return c + (1.0 - c) * pow(1.0 - VdotH, 5.0);
}

float dBeckmann(float NdotH) {
	//float roughness = texture(gDiff, fragUV).a;
	float a2 = pow(roughness, 4);
	float nd = NdotH * NdotH;

	float d = nd * (a2 -1.0) + 1.0;
	d = PI * d * d;
	return a2 / d;
}

float gCookTorrance(float NdotV, float VdotH, float NdotH) {
	float v1 = 2.0 * NdotH * NdotV / VdotH;
	return min(1, v1);
}

/* Specular*/

/* Diffuse */
vec3 lambert(float NdotL, vec3 matDiffuse) {
	//diffuse = light.diffuse * NdotL * material.diffuse;
	return vec3(1.0) * NdotL * matDiffuse;
}

vec4 calcColor() {
	//float roughness = texture(gDiff, fragUV).a;
	//float metalness = texture(gPos, fragUV).a;

	vec4 color = vec4(0.0);

	vec3 ligthPos = vec3(42.0, 44.0, 22.0);
	vec3 worldPos = texture(gPos, fragUV).rgb;

	vec3 L = normalize(ligthPos - worldPos);
	vec3 V = normalize(viewerPosition - worldPos);
	vec3 H = normalize(L + V);
	vec3 N = normalize(texture(gNorm, fragUV).xyz);

	float VdotH = max(0.0, dot(V, H));
	float NdotH = max(0.0, dot(N, H));
	float NdotV = max(0.0, dot(N, V));
	float NdotL = max(0.0, dot(N, L));

	vec4 albedo = vec4(texture(gDiff, fragUV).rgb, 1.0);

	// Fresnel Coefficient : Gold
	vec3 c = mix(vec3(0.96, 0.93, 0.88), albedo.rgb, 0.0);

	vec3 diffuse = albedo.rgb * clamp(NdotL, 0.0, 1.0) * (1.0 - metalness);
	// ambient color + PBR
	vec3 nom = fresnel(VdotH, c) *
						 dBeckmann(NdotH) *
						 gCookTorrance(NdotV, VdotH, NdotH) * 0.05;

	vec3 specular = ((1.0 - metalness) + metalness * albedo.rgb) * (nom / 4.0 * NdotV * NdotL);

	float dist = length(ligthPos - worldPos);
	float attenuation = 3000.0 / (dist * dist);

	color.rgb = vec3(0.3,0.15,0.0) * (diffuse + specular) * attenuation;

	color.rgb /= 1.0; //Knum lights
	color.rgb += vec3(0.05) * albedo.rgb;

	return vec4(color.rgb, 1.0);
}

void main() {
	fragColor = calcColor();
}
