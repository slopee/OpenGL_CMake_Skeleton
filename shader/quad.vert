#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 gridScale;
uniform sampler2D textureSampler;

out vec2 fUv;

void main(void)
{
		fUv = uv;
		vec4 heightmap = texture(textureSampler, fUv);
		vec3 height = vec3(0, 0, mix(heightmap.r, heightmap.b, heightmap.b) * 30);
    vec4 newPosition = view * gridScale * vec4(position + height,1.0);
    gl_Position = projection * newPosition;
}
