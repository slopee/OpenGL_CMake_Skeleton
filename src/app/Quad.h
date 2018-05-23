#pragma once
#include <graphic/Shader.hpp>
#include <graphic/ShaderProgram.h>

class Quad
{
public:
	Quad(float size);

	void Draw(float time, const glm::mat4& projection, const glm::mat4& view);

	void SetSize(float newSize);

private:
	float m_Size;

	Shader m_VertexShader;
	Shader m_FragmentShader;
	ShaderProgram m_ShaderProgram;

	GLuint m_Vao; 
	GLuint m_Vbo;
	GLuint m_Ibo;
};
