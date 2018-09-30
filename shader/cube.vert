#version 450

in vec3 position;
in vec4 color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 transformation;
uniform float size;
uniform vec3 faceColor;

out vec4 fColor;

void main(void)
{
		fColor = vec4(faceColor, 1.0f);
	  vec4 localPosition = transformation * vec4(position, 1.0);

		float positionLength = length(localPosition);
		localPosition.xyz /= positionLength;
		localPosition.xyz *= size;

    vec4 newPosition = view * localPosition;
    gl_Position = projection * newPosition;
}
