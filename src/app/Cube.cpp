#include "Cube.h"
#include "Grid.h"
#include <vector>
#include <glm/gtc/matrix_transform.inl>
#include "graphic/Transform.h"

//---------------------------------------------------------------------------------------------------------------------
Cube::Cube(float size, Grid* const grid) :
	m_Size(size),
	m_Grid(grid),
	m_VertexShader(SHADER_DIR"/cube.vert", GL_VERTEX_SHADER),
	m_FragmentShader(SHADER_DIR"/cube.frag", GL_FRAGMENT_SHADER),
	m_ShaderProgram({ m_VertexShader,m_FragmentShader })
{
	const auto& gridSize = m_Grid->GetSize();
	const glm::vec2 gridHalfSize(gridSize.x / 2.0f, gridSize.y / 2.0f);

	// Front
	Transform frontFace;
	frontFace.position = glm::vec3(0, 0, gridHalfSize.y);
	m_FacesTransforms.push_back(frontFace);
	
	//Left
	Transform leftFace;
	leftFace.position = glm::vec3(gridHalfSize.x, 0, 0);
	leftFace.rotation = glm::vec3(0, -90.0f, 0);
	//leftFace.scale = glm::vec3(2.0f, 2.0f, 2.0f);
	m_FacesTransforms.push_back(leftFace);

	//Right
	Transform rightFace;
	rightFace.position = glm::vec3(-gridHalfSize.x, 0, 0);
	rightFace.rotation = glm::vec3(0, 90.0f, 0);
	m_FacesTransforms.push_back(rightFace);

	//Back
	Transform backFace;
	backFace.position = glm::vec3(0, 0, -gridHalfSize.y);
	m_FacesTransforms.push_back(backFace);

	// Top
	Transform topFace;
	topFace.position = glm::vec3(0, gridHalfSize.y, 0);
	topFace.rotation = glm::vec3(90.0f, 0, 0);
	m_FacesTransforms.push_back(topFace);

	// Bottom
	Transform bottomFace;
	bottomFace.position = glm::vec3(0, -gridHalfSize.y, 0);
	bottomFace.rotation = glm::vec3(90.0f, 0, 0);
	m_FacesTransforms.push_back(bottomFace);
}

//---------------------------------------------------------------------------------------------------------------------
void Cube::Draw(float time, const glm::mat4& projection, const glm::mat4& view)
{
	m_ShaderProgram.use();

	// Set the uniforms
	m_ShaderProgram.setUniform("projection", projection);
	m_ShaderProgram.setUniform("view", view);
	m_ShaderProgram.setUniform("size", m_Size);	
	
	glm::vec3 colors[6] {
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.5f, 0.5f, 0.0f),
		glm::vec3(0.0f, 0.5f, 0.5f),
		glm::vec3(1.0f, 0.5f, 0.5f),
	};
	
	int i = 0;
	for(auto& transform : m_FacesTransforms)
	{
		m_ShaderProgram.setUniform("faceColor", colors[i]);
		m_ShaderProgram.setUniform("transformation", transform.GetTransformMatrix());
		m_Grid->DrawMesh();
		++i;
	}
	
	// Unbind program
	m_ShaderProgram.unuse();
}


