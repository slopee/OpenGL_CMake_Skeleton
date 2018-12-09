#pragma once
#include <graphic/Shader.hpp>
#include <graphic/ShaderProgram.h>

class Transform;

class Grid
{
public:
	Grid(glm::uvec2 size);

	void Draw(float time, const glm::mat4& projection, const glm::mat4& view, const Transform& localTransform);
	void DrawMesh() const;

	const glm::uvec2& GetSize() const;
private:
	glm::uvec2 m_Size;

	int m_TotalIndices;

	Shader m_VertexShader;
	Shader m_FragmentShader;
	ShaderProgram m_ShaderProgram;

	GLuint m_Vao;
};