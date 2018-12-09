#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

uniform mat4 projection;
uniform mat4 view;
uniform float size;

uniform mat4 treeTransformation;

uniform mat4 gridScale;
uniform vec3 nodeColor;
uniform mat4 nodeTransformation;
uniform sampler2D textureSampler;
uniform vec4 uvLimits;

out vec4 fColor;

void main(void)
{
		fColor = vec4(nodeColor, 1.0f);

		float u = mix(uvLimits.x, uvLimits.z, uv.x);
		float v = mix(uvLimits.y, uvLimits.w, uv.y);

		vec4 heightmap = texture(textureSampler, vec2(u, v));
		vec3 height = vec3(0, 0, mix(heightmap.r, heightmap.b, heightmap.b) * -40);
	  vec4 localPosition = treeTransformation * nodeTransformation * gridScale * vec4(position + height, 1.0);

		/*
		float positionLength = length(localPosition);
		localPosition.xyz /= positionLength;
		localPosition.xyz *= size;
		*/

    vec4 newPosition = view * localPosition;
    gl_Position = projection * newPosition;
}
