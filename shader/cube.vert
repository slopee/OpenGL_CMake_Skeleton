#version 450

in vec3 position;
in vec4 color;

uniform mat4 projection;
uniform mat4 view;
uniform float size;

out vec4 fColor;

void main(void)
{
	fColor = color;
    vec4 newPosition = view * vec4(position*size,1.0);
    gl_Position = projection * newPosition;
}
