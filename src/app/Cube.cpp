#include "Cube.h"
#include <vector>

struct VertexInfo
{
	glm::vec3 position;
	glm::vec4 color;

	VertexInfo(glm::vec3 pos, glm::vec3 col) : position(pos), color(glm::vec4(col.x, col.y, col.z, 1.0)) {}
};


Cube::Cube(float size) :
	m_Size(size),
	m_VertexShader(SHADER_DIR"/cube.vert", GL_VERTEX_SHADER),
	m_FragmentShader(SHADER_DIR"/cube.frag", GL_FRAGMENT_SHADER),
	m_ShaderProgram({ m_VertexShader,m_FragmentShader })
{
	std::vector<VertexInfo> vertices;
	std::vector<GLuint> indices;

	// Front Face
	vertices.emplace_back(VertexInfo(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, 1.0, 0.0)));
	vertices.emplace_back(VertexInfo(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0, 1.0, 0.0)));
	vertices.emplace_back(VertexInfo(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0, 1.0, 0.0)));
	vertices.emplace_back(VertexInfo(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.0, 1.0, 0.0)));


	indices.emplace_back(0);
	indices.emplace_back(1);
	indices.emplace_back(2);
	indices.emplace_back(1);
	indices.emplace_back(3);
	indices.emplace_back(2);

	// Right Face
	vertices.emplace_back(VertexInfo(glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0, 0.0, 0.0)));
	vertices.emplace_back(VertexInfo(glm::vec3(1.0f, 0.0f, -1.0f), glm::vec3(1.0, 0.0, 0.0)));
	vertices.emplace_back(VertexInfo(glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(1.0, 0.0, 0.0)));
	vertices.emplace_back(VertexInfo(glm::vec3(1.0f, 1.0f, -1.0f), glm::vec3(1.0, 0.0, 0.0)));

	indices.emplace_back(0 + 4);
	indices.emplace_back(1 + 4);
	indices.emplace_back(2 + 4);
	indices.emplace_back(1 + 4);
	indices.emplace_back(3 + 4);
	indices.emplace_back(2 + 4);

	// Left Face
	vertices.emplace_back(VertexInfo(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0, 0.0, 1.0)));
	vertices.emplace_back(VertexInfo(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0, 0.0, 1.0)));
	vertices.emplace_back(VertexInfo(glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0, 0.0, 1.0)));
	vertices.emplace_back(VertexInfo(glm::vec3(0.0f, 1.0f, -1.0f), glm::vec3(0.0, 0.0, 1.0)));

	indices.emplace_back(0 + 8);
	indices.emplace_back(1 + 8);
	indices.emplace_back(2 + 8);
	indices.emplace_back(1 + 8);
	indices.emplace_back(3 + 8);
	indices.emplace_back(2 + 8);

	glGenVertexArrays(1, &m_Vao);
	glBindVertexArray(m_Vao);

	GLuint vbo;
	GLuint ibo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexInfo), vertices.data(), GL_STATIC_DRAW);

	m_ShaderProgram.setAttribute("position", 3, sizeof(VertexInfo), offsetof(VertexInfo, position));
	m_ShaderProgram.setAttribute("color", 4, sizeof(VertexInfo), offsetof(VertexInfo, color));

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

void Cube::Draw(float time, const glm::mat4& projection, const glm::mat4& view)
{
	m_ShaderProgram.use();

	// Set the uniforms
	m_ShaderProgram.setUniform("projection", projection);
	m_ShaderProgram.setUniform("view", view);

	// Bind VAO
	glBindVertexArray(m_Vao);
	
	m_ShaderProgram.setUniform("size", m_Size);

	// Draw Triangles depending on the size
	glDrawElements(
		GL_TRIANGLES,      // mode
		18,         // count
		GL_UNSIGNED_INT,   // type
		NULL               // element array buffer offset
	);

	// Unbind VAO
	glBindVertexArray(0);

	// Unbind program
	m_ShaderProgram.unuse();
}


