#pragma once
#include <graphic/Shader.hpp>
#include <graphic/ShaderProgram.h>

class Transform;

class Grid
{
public:
	Grid(glm::ivec2 size);

	void Draw(float time, const glm::mat4& projection, const glm::mat4& view, const Transform& localTransform);
	void DrawMesh() const;

	const glm::ivec2& GetSize() const;


private:
	glm::ivec2 m_Size;

	int m_TotalIndices;

	Shader m_VertexShader;
	Shader m_FragmentShader;
	ShaderProgram m_ShaderProgram;

	GLuint m_Vao;
};