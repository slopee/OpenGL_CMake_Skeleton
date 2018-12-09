#version 450

uniform sampler2D textureSampler;

in vec4 fColor;
in vec2 fUv;
// output
out vec4 color;

void main(void)
{
    //color = fColor;
    color = vec4(texture(textureSampler, fUv).rgb, 1.0f);
}
