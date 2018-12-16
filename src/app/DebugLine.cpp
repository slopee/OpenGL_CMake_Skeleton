#include "DebugLine.h"
#include <vector>

static GLuint m_Vao;
static GLuint m_Vbo;
//static Shader m_VertexShader = Shader(SHADER_DIR"/line_debug.vert", GL_VERTEX_SHADER);
//static Shader m_FragmentShader = Shader(SHADER_DIR"/line_debug.frag", GL_FRAGMENT_SHADER);
//static ShaderProgram m_ShaderProgram = ShaderProgram({ m_VertexShader, m_FragmentShader });

void DebugLine::DrawLine(const glm::vec3& origin, const glm::vec3& dest, const Camera& camera)
{
	/*
	if(m_Vao == 0)
	{
		InitOpenGLContext();
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
	std::vector<glm::vec3> vertices { origin, dest };
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_DYNAMIC_DRAW);

	m_ShaderProgram.use();

	// Bind VAO
	glBindVertexArray(m_Vao);

	// Draw Triangles depending on the size
	glDrawElements(
		GL_LINES,      // mode
		2,         // count
		GL_UNSIGNED_INT,   // type
		NULL               // element array buffer offset
	);

	// Unbind VAO
	glBindVertexArray(0);

	m_ShaderProgram.unuse();
	*/
}

//---------------------------------------------------------------------------------------------------------------------
void DebugLine::InitOpenGLContext()
{
	glGenVertexArrays(1, &m_Vao);
	glBindVertexArray(m_Vao);

	glGenBuffers(1, &m_Vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
	glBindVertexBuffer(0, m_Vbo, 0, sizeof(glm::vec3));

	// Position will always go to location 0
	glEnableVertexAttribArray(0);
	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
	glVertexAttribBinding(0, 0);

	// IBO
	std::vector<GLuint> indices;
	indices.push_back(0);
	indices.push_back(1);

	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}
