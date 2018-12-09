#include "Quad.h"
#include <app/Grid.h>
#include <glm/gtc/matrix_transform.inl>

Quad::Quad(float size, Grid* grid) :
	Renderable(
		Shader(SHADER_DIR"/quad.vert", GL_VERTEX_SHADER), 
		Shader(SHADER_DIR"/quad.frag", GL_FRAGMENT_SHADER)),
	m_Size(size),
	m_Grid(grid)
{	
}

void Quad::OnDraw(float time, const Camera& camera)
{
	const auto gridSize = m_Grid->GetSize();
	glm::mat4 gridScale = glm::scale(glm::mat4(), glm::vec3(m_Size / gridSize.x, m_Size / gridSize.y, 1.0f));
	m_ShaderProgram.setUniform("gridScale", gridScale);
	
	m_Grid->DrawMesh();
}
