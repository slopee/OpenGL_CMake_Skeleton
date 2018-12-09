#include "CameraDebugDrawer.h"
#include <glm/gtx/transform.hpp>
#include <graphic/Transform.h>

static std::vector<GLuint> indices;

//---------------------------------------------------------------------------------------------------------------------
CameraDebugDrawer::CameraDebugDrawer(
	const Camera::LookAtVectors& cameraLookAtVector,
	float fov,
	float nearDist,
	float farDist,
	float ratio) :
	m_ShaderProgram({ 
		Shader(SHADER_DIR"/camera_debug.vert", GL_VERTEX_SHADER),
		Shader(SHADER_DIR"/camera_debug.frag", GL_FRAGMENT_SHADER) })
{
	glGenVertexArrays(1, &m_Vao);
	glBindVertexArray(m_Vao);

	auto vertices = CalculateVerticesPositions(cameraLookAtVector, fov, nearDist, farDist, ratio);
		
	// Look Vector
	indices.push_back(0);
	indices.push_back(1);

	// Up vector
	indices.push_back(0);
	indices.push_back(2);
	
	// Near Plane	
	indices.push_back(0);
	indices.push_back(3);
	indices.push_back(0);
	indices.push_back(4);
	indices.push_back(0);
	indices.push_back(5);
	indices.push_back(0);
	indices.push_back(6);	
	indices.push_back(3);
	indices.push_back(4);
	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(5);
	indices.push_back(6);
	indices.push_back(6);
	indices.push_back(3);
	
	// Far Plane
	indices.push_back(0);
	indices.push_back(7);
	indices.push_back(0);
	indices.push_back(8);
	indices.push_back(0);
	indices.push_back(9);
	indices.push_back(0);
	indices.push_back(10);
	indices.push_back(7);
	indices.push_back(8);
	indices.push_back(8);
	indices.push_back(9);
	indices.push_back(9);
	indices.push_back(10);
	indices.push_back(10);
	indices.push_back(7);
	
	GLuint ibo;
	glGenBuffers(1, &m_Vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_DYNAMIC_DRAW);

	glBindVertexBuffer(0, m_Vbo, 0, sizeof(glm::vec3));

	// Position will always go to location 0
	glEnableVertexAttribArray(0);
	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(0, 0);
	
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

//---------------------------------------------------------------------------------------------------------------------
void CameraDebugDrawer::Update(
	const Camera::LookAtVectors& cameraLookAtVector,
	float fov, 
	float nearDist, 
	float farDist, 
	float ratio)
{
	auto vertices = CalculateVerticesPositions(cameraLookAtVector, fov, nearDist, farDist, ratio);
		
	glBindVertexArray(m_Vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
	
	glBufferSubData(GL_ARRAY_BUFFER, 0, vertices.size() * sizeof(glm::vec3), vertices.data());

	glBindVertexArray(0);
}

//---------------------------------------------------------------------------------------------------------------------
void CameraDebugDrawer::Draw(float time, const glm::mat4& projection, const glm::mat4& view)
{
	m_ShaderProgram.use();
	
	// Set the uniforms
	m_ShaderProgram.setUniform("projection", projection);
	m_ShaderProgram.setUniform("view", view);

	// Bind VAO
	glBindVertexArray(m_Vao);
	
	// Draw Triangles depending on the size
	glDrawElements(
		GL_LINES,      // mode
		indices.size(),         // count
		GL_UNSIGNED_INT,   // type
		NULL               // element array buffer offset
	);

	// Unbind VAO
	glBindVertexArray(0);

	// Unbind program
	m_ShaderProgram.unuse();
}

//---------------------------------------------------------------------------------------------------------------------
std::vector<glm::vec3> CameraDebugDrawer::CalculateVerticesPositions(
	const Camera::LookAtVectors& cameraLookAtVector,
	float fov, 
	float nearDist, 
	float farDist, 
	float ratio)
{
	std::vector<glm::vec3> vertices;	
	
	const auto& cameraPosition = cameraLookAtVector.position;
	const auto& cameraDirection = glm::normalize(cameraLookAtVector.viewDirection);
	const auto& cameraUp = glm::normalize(cameraLookAtVector.up);
	const auto cameraRight = cameraLookAtVector.CalculateRight();
	
	vertices.push_back(cameraPosition); // 0
	vertices.push_back(cameraPosition + cameraDirection); // 1		
	vertices.push_back(cameraPosition + cameraUp); // 2
		
	// Near plane
	nearDist -= 0.05f;
	const float nearHeight = 2 * tan(fov / 2) * nearDist;
	const float nearWidth = nearHeight * ratio;
	const auto nearCenter = cameraPosition + (cameraDirection * nearDist);

	vertices.push_back(nearCenter + (cameraUp * nearHeight / 2.0f) - (cameraRight * nearWidth / 2.0f)); // 3
	vertices.push_back(nearCenter + (cameraUp * nearHeight / 2.0f) + (cameraRight * nearWidth / 2.0f)); // 4
	vertices.push_back(nearCenter - (cameraUp * nearHeight / 2.0f) + (cameraRight * nearWidth / 2.0f)); // 5
	vertices.push_back(nearCenter - (cameraUp * nearHeight / 2.0f) - (cameraRight * nearWidth / 2.0f)); // 6
																									
	// Far plane
	farDist -= 0.05f;
	const float farHeight = 2 * tan(fov / 2) * farDist;
	const float farWidth = farHeight * ratio;
	const auto farCenter = cameraDirection + (cameraDirection * farDist);

	vertices.push_back(farCenter + (cameraUp * farHeight / 2.0f) - (cameraRight * farWidth / 2.0f)); // 7
	vertices.push_back(farCenter + (cameraUp * farHeight / 2.0f) + (cameraRight * farWidth / 2.0f)); // 8
	vertices.push_back(farCenter - (cameraUp * farHeight / 2.0f) + (cameraRight * farWidth / 2.0f)); // 9
	vertices.push_back(farCenter - (cameraUp * farHeight / 2.0f) - (cameraRight * farWidth / 2.0f)); // 10	
	return vertices;
}
