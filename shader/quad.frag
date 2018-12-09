#version 450

uniform sampler2D textureSampler;

in vec2 fUv;
// output
out vec4 color;

void main(void)
{
    //color = fColor;
    color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
}
