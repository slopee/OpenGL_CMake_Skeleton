#pragma once
#include <graphic/Shader.hpp>
#include <graphic/ShaderProgram.h>

class Grid
{
public:
	Grid(glm::ivec2 size, glm::vec3 center);

	void Draw(float time, const glm::mat4& projection, const glm::mat4& view);


private:
	glm::ivec2 m_Size;

	Shader m_VertexShader;
	Shader m_FragmentShader;
	ShaderProgram m_ShaderProgram;

	GLuint m_Vao;
};