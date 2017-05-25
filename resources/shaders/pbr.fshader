#version 330

const int maxlights = 2;
const float PI = 3.1415926536;

struct Light {
	int type; // 0 : Directional,  1 : Pointlight
	vec3 amb;
	vec3 pos;// Or direction in case of directional
};
Light lights[maxlights] = Light[maxlights](
	 Light(1, vec3(0.3, 0.15, 0.0), vec3(42.0, 44.0, 22.0)),
	 Light(0, vec3(0.5, 0.5, 2.5), vec3(0.75, -0.75, -0.75))
);
// pass for uniforms
const float roughness = 0.45;
const float metalness = 0.3;

in vec2 fragUV;

uniform vec3 viewerPosition;

uniform sampler2D gDiff;
uniform sampler2D gNorm;
uniform sampler2D gPos;
uniform sampler2D gSpec;

out vec4 lightColor;
out vec4 luminance;

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

vec4 calcColor(Light l) {
	vec4 color = vec4(0.0);

	vec3 worldPos = texture(gPos, fragUV).rgb;

	vec3 L = l.type == 0 ? normalize(-l.pos) : normalize(l.pos - worldPos);
	vec3 V = normalize(viewerPosition - worldPos);
	vec3 H = normalize(L + V);
	vec3 N = normalize(texture(gNorm, fragUV).xyz);

	float VdotH = max(0.0, dot(V, H));
	float NdotH = max(0.0, dot(N, H));
	float NdotV = max(0.0, dot(N, V));
	float NdotL = max(0.0, dot(N, L));

	vec4 albedo = vec4(texture(gDiff, fragUV).rgb, 1.0);

	vec3 diffuse = albedo.rgb * clamp(NdotL, 0.0, 1.0) * (1.0 - metalness);

	// ambient color + PBR
	vec3 nom = fresnel(VdotH, vec3(1.0)) *
						 dBeckmann(NdotH) *
						 gCookTorrance(NdotV, VdotH, NdotH);

	vec3 specular = ((1.0 - metalness) + metalness * albedo.rgb) * (nom / 4.0 * NdotV * NdotL);

	float dist = length(l.pos - worldPos);

	// Don't have attenuation if it's directional
	float attenuation = (l.type == 0) ?  1.0 : (5000.0 / (dist * dist));

	float red = texture(gSpec, fragUV).r;
	specular *= l.type == 0 ? 1.0 : red;
	if (red > 0.7) {
		diffuse = vec3(0.1);
	}
	color.rgb = l.amb * (diffuse + specular) * attenuation;
	color.rgb /= maxlights; //Knum lights
	return vec4(color.rgb, 1.0);
}

void main() {
	for (int i = 0; i < maxlights; ++i) {
		lightColor += calcColor(lights[i]);
	}
 // Code for luminance
		if (dot(lightColor.rgb, vec3(0.2126, 0.7152, 0.0722)) > 0.5) {
				luminance = vec4(lightColor.rgb, 1.0);
		}  else {
				luminance = vec4(0.0, 0.0, 0.0, 0.0);
		}
}
