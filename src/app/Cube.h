#pragma once
#include <graphic/Shader.hpp>
#include <graphic/ShaderProgram.h>

class Cube
{
public:
	Cube(float size);

	void Draw(float time, const glm::mat4& projection, const glm::mat4& view);

private:
	float m_Size;

	Shader m_VertexShader;
	Shader m_FragmentShader;
	ShaderProgram m_ShaderProgram;

	GLuint m_Vao;
};