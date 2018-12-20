#version 460

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 scale;
layout (location = 3) in vec3 nodeColor;
layout (location = 4) in vec3 translation;
layout (location = 5) in vec4 uvLimits;

uniform mat4 projection;
uniform mat4 view;

uniform mat4 treeTransformation;

uniform sampler2D textureSampler;

out vec4 fColor;

void main(void)
{
		fColor = vec4(nodeColor, 1.0f);

		float u = mix(uvLimits.x, uvLimits.z, uv.x);
		float v = mix(uvLimits.y, uvLimits.w, uv.y);

		vec4 heightmap = texture(textureSampler, vec2(u, v));
		vec3 height = vec3(0, 0, mix(heightmap.r, heightmap.b, heightmap.b) * -40);
	  vec4 localPosition = treeTransformation * (vec4(translation, 1.0f) + (vec4(scale, 1.0f) * vec4(position + height, 1.0)));
		//vec4 localPosition = treeTransformation * vec4(position, 1.0);

    vec4 newPosition = view * localPosition;
    gl_Position = projection * newPosition;
}
