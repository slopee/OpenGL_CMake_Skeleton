#pragma once
#include <graphic/Shader.hpp>
#include <graphic/ShaderProgram.h>
#include <vector>
#include "Grid.h"

class Grid;

class Cube
{
public:
	Cube(float size, Grid * const grid);

	void Draw(float time, const glm::mat4& projection, const glm::mat4& view);

private:
	float m_Size;
	Grid* const m_Grid;

	Shader m_VertexShader;
	Shader m_FragmentShader;
	ShaderProgram m_ShaderProgram;

	std::vector<Transform> m_FacesTransforms;
};