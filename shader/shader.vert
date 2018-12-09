#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec4 color;

uniform mat4 projection;
uniform mat4 view;

out vec4 fColor;

void main(void)
{
		fColor = color;
    vec4 newPosition = view * vec4(position,1.0);
    gl_Position = projection * newPosition;
}
