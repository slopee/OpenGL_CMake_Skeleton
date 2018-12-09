#pragma once
#include <graphic/Shader.hpp>
#include <graphic/ShaderProgram.h>
#include <vector>
#include "Grid.h"

class Grid;
class QuadTree;

class Cube
{
public:
	Cube(float size, Grid * const grid);

	void Draw(float time, const glm::mat4& projection, const glm::mat4& view, const glm::vec3& cameraPosition);

private:
	float m_Size;
	Grid* const m_Grid;

	Shader m_VertexShader;
	Shader m_FragmentShader;
	ShaderProgram m_ShaderProgram;

	Shader m_QuadTreeVertexShader;
	Shader m_QuadTreeFragmentShader;
	ShaderProgram m_QuadTreeShaderProgram;

	QuadTree* m_QuadTree;

	std::vector<Transform> m_FacesTransforms;
};