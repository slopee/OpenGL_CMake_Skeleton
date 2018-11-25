#pragma once
#include <graphic/Shader.hpp>
#include <graphic/ShaderProgram.h>
#include "graphic/Transform.h"

class Axis
{
public:
	Axis();

	void Draw(float time, const glm::mat4& projection, const glm::vec3& cameraRotation);	

private:
	Transform m_Transform;

	Shader m_VertexShader;
	Shader m_FragmentShader;
	ShaderProgram m_ShaderProgram;

	GLuint m_Vao; 
	GLuint m_Vbo;
	GLuint m_Ibo;
};
