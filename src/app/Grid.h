#pragma once
#include <graphic/Shader.hpp>
#include <graphic/ShaderProgram.h>

struct Transform
{
	glm::vec3 position;
	glm::vec3 rotation;
	glm::vec3 scale;
};

class Grid
{
public:
	Grid(glm::ivec2 size);

	void Draw(float time, const glm::mat4& projection, const glm::mat4& view, const Transform& localTransform);


private:
	glm::ivec2 m_Size;

	int m_TotalIndices;

	Shader m_VertexShader;
	Shader m_FragmentShader;
	ShaderProgram m_ShaderProgram;

	GLuint m_Vao;
};