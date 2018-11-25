#include "Axis.h"
#include <vector>

struct VertexInfo
{
	glm::vec3 position;
	glm::vec4 color;

	VertexInfo(glm::vec3 pos, glm::vec3 color) : position(pos), color(glm::vec4(color, 1.0)) {}
};

Axis::Axis() :
	m_Transform(),
	m_VertexShader(SHADER_DIR"/shader.vert", GL_VERTEX_SHADER),
	m_FragmentShader(SHADER_DIR"/shader.frag", GL_FRAGMENT_SHADER),
	m_ShaderProgram({ m_VertexShader,m_FragmentShader })
{
	m_Transform.position = glm::vec3(8.5f, 4.5f, -10.0f);

	std::vector<VertexInfo> vertices;
	std::vector<GLuint> indices;

	vertices.emplace_back(VertexInfo(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	vertices.emplace_back(VertexInfo(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
	vertices.emplace_back(VertexInfo(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	vertices.emplace_back(VertexInfo(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)));
	vertices.emplace_back(VertexInfo(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
	vertices.emplace_back(VertexInfo(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, 1.0f)));

	indices.emplace_back(0);
	indices.emplace_back(1);
	indices.emplace_back(2);
	indices.emplace_back(3);
	indices.emplace_back(4);
	indices.emplace_back(5);
	
	glGenVertexArrays(1, &m_Vao);
	glBindVertexArray(m_Vao);

	glGenBuffers(1, &m_Vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexInfo), vertices.data(), GL_STATIC_DRAW);

	m_ShaderProgram.setAttribute("position", 3, sizeof(VertexInfo), offsetof(VertexInfo, position));
	m_ShaderProgram.setAttribute("color", 4, sizeof(VertexInfo), offsetof(VertexInfo, color));

	glGenBuffers(1, &m_Ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
	
	glBindVertexArray(0);
}

void Axis::Draw(float time, const glm::mat4& projection, const glm::vec3& cameraRotation)
{
	m_ShaderProgram.use();

	m_Transform.rotation = cameraRotation;

	// Set the uniforms
	m_ShaderProgram.setUniform("projection", projection);
	m_ShaderProgram.setUniform("view", m_Transform.GetTransformMatrix());

	// Bind VAO
	glBindVertexArray(m_Vao);

	// Draw Triangles depending on the size
	glDrawElements(
		GL_LINES,      // mode
		6,         // count
		GL_UNSIGNED_INT,   // type
		NULL               // element array buffer offset
	);

	// Unbind VAO
	glBindVertexArray(0);

	// Unbind program
	m_ShaderProgram.unuse();
}