#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTextCoord;

out vec2 TextCoord;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;

void main() {
	gl_Position = projMatrix * viewMatrix * modelMatrix * vec4(inPos,1.0f);
	TextCoord = vec2(inTextCoord.x, inTextCoord.y);
}