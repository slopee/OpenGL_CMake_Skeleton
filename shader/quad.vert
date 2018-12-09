#version 450

in vec3 position;
in vec4 color;
in vec2 uv;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 gridScale;

out vec4 fColor;
out vec2 fUv;

void main(void)
{
		fColor = color;
		fUv = uv;
    vec4 newPosition = view * gridScale * vec4(position,1.0);
    gl_Position = projection * newPosition;
}
