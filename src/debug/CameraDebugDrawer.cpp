#include "CameraDebugDrawer.h"
#include <glm/gtx/transform.hpp>
#include <graphic/Transform.h>

static std::vector<GLuint> indices;

//---------------------------------------------------------------------------------------------------------------------
CameraDebugDrawer::CameraDebugDrawer(
	const Transform& cameraTransform,
	float fov,
	float nearDist,
	float farDist,
	float ratio) :
	m_ShaderProgram({ 
		Shader(SHADER_DIR"/axis.vert", GL_VERTEX_SHADER),
		Shader(SHADER_DIR"/axis.frag", GL_FRAGMENT_SHADER) })
{
	glGenVertexArrays(1, &m_Vao);
	glBindVertexArray(m_Vao);

	auto vertices = CalculateVerticesPositions(cameraTransform, fov, nearDist, farDist, ratio);
		
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
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);

	m_ShaderProgram.setAttribute("position", 3, sizeof(glm::vec3), 0);

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

//---------------------------------------------------------------------------------------------------------------------
void CameraDebugDrawer::Update(
	const Transform& cameraTransform,
	float fov, 
	float nearDist, 
	float farDist, 
	float ratio)
{
	auto vertices = CalculateVerticesPositions(cameraTransform, fov, nearDist, farDist, ratio);
		
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
	const Transform& cameraTransform,
	float fov, 
	float nearDist, 
	float farDist, 
	float ratio)
{
	std::vector<glm::vec3> vertices;	

	const auto inverseTransform = cameraTransform.GetInverseTransformMatrix();

	const auto cameraPosition = glm::vec3(inverseTransform * glm::vec4(0.0f, 0.0f, 0.0, 1.0f));
	const auto cameraDirection = glm::vec3(inverseTransform * glm::vec4(0.0f, 0.0f, -1.0f, 1.0f));
	const auto cameraUp = glm::vec3(inverseTransform * glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));

	vertices.push_back(cameraPosition); // 0
	vertices.push_back(cameraDirection); // 1		
	vertices.push_back(cameraUp); // 2
		
	// Near plane
	nearDist -= 0.05f;
	const float nearHeight = 2 * tan(fov / 2) * nearDist;
	const float nearWidth = nearHeight * ratio;
	const auto direction = cameraDirection - cameraPosition;
	const float nearZ = cameraPosition.z + (direction.z * nearDist);

	vertices.push_back(glm::vec3(cameraDirection.x - nearWidth / 2, cameraDirection.y + nearHeight / 2, nearZ)); // 3
	vertices.push_back(glm::vec3(cameraDirection.x + nearWidth / 2, cameraDirection.y + nearHeight / 2, nearZ)); // 4
	vertices.push_back(glm::vec3(cameraDirection.x + nearWidth / 2, cameraDirection.y - nearHeight / 2, nearZ)); // 5
	vertices.push_back(glm::vec3(cameraDirection.x - nearWidth / 2, cameraDirection.y - nearHeight / 2, nearZ)); // 6
	
	// Far plane
	farDist -= 0.05f;
	const float farHeight = 2 * tan(fov / 2) * farDist;
	const float farWidth = farHeight * ratio;
	const float farZ = cameraDirection.z + (direction.z * farDist);

	vertices.push_back(glm::vec3(cameraDirection.x - farWidth / 2, cameraDirection.y + farHeight / 2, farZ)); // 7
	vertices.push_back(glm::vec3(cameraDirection.x + farWidth / 2, cameraDirection.y + farHeight / 2, farZ)); // 8
	vertices.push_back(glm::vec3(cameraDirection.x + farWidth / 2, cameraDirection.y - farHeight / 2, farZ)); // 9
	vertices.push_back(glm::vec3(cameraDirection.x - farWidth / 2, cameraDirection.y - farHeight / 2, farZ)); // 10
	
	return vertices;
}
