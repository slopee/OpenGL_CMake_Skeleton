#pragma once
#include <graphic/Shader.hpp>
#include <graphic/ShaderProgram.h>

class Transform;

class Grid
{
public:
	Grid(glm::uvec2 size);
		
	void DrawMesh() const;

	const glm::uvec2& GetSize() const;
private:
	glm::uvec2 m_Size;

	int m_TotalIndices;
		
	GLuint m_Vao;
};