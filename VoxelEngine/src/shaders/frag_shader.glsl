#version 330 core
out vec4 FragColor;

in vec2 TextCoord;

uniform sampler2D texture1;

void main() {
	FragColor = texture(texture1, TextCoord);
	//FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}