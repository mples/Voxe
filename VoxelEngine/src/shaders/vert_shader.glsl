#version 330 core
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec2 inTextCoord;

out vec2 TextCoord;

void main() {
	gl_Position = vec4(inPos,1.0f);
	TextCoord = vec2(inTextCoord.x, inTextCoord.y);
}