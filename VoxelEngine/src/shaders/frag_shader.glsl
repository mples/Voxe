#version 330 core
out vec4 FragColor;

in vec2 TextCoord;

uniform sampler2D texture1;
const vec4 fogColor = vec4(0.3, 0.3, 0.3, 1.0);
const float fogDens = 0.00003;
void main() {
	FragColor = texture(texture1, TextCoord);

	float z = gl_FragCoord.z /gl_FragCoord.w;
	float fog = clamp(exp(-fogDens * z * z), 0.2, 1);
	
	FragColor = mix(fogColor, FragColor, fog);
	//FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}