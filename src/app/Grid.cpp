#include "Grid.h"
#include <vector>
#include <glm/gtc/matrix_transform.inl>
#include "graphic/Transform.h"

struct VertexInfo
{
	glm::vec3 position;
	glm::vec2 uv;

	VertexInfo(glm::vec3 pos, glm::vec2 uv) : position(pos), uv(uv) {}
};

//---------------------------------------------------------------------------------------------------------------------
Grid::Grid(glm::uvec2 size) :
	m_Size(size)
{
	int totalVerticesSize = (size.x + 1) * (size.y + 1);
	glm::vec2 halfSize(size.x / 2.0f, size.y / 2.0f);

	std::vector<VertexInfo> vertices;
	vertices.reserve(totalVerticesSize);

	std::vector<GLuint> indices;
	indices.reserve(totalVerticesSize * 4);
	
	/* Init algorithm to create vertices */
	float currentY = 0.0f;
	float currentX = 0.0f;
	int verticesPerRow = 0;
	for(int i = 0; i < totalVerticesSize; ++i)
	{
		vertices.push_back(VertexInfo(glm::vec3(currentX - halfSize.x, currentY - halfSize.y, 0.0f), glm::vec2(currentX/size.x, currentY/size.y)));
		++verticesPerRow;

		if(verticesPerRow == (size.x + 1))
		{
			++currentY;
			currentX = 0.0f;
			verticesPerRow = 0;
		}
		else
		{
			++currentX;
		}
	}

	for(int indexY = 0; indexY < size.y; ++indexY)
	{
		for (int indexX = 0; indexX < size.x; ++indexX)
		{
			int currentIndex = ((size.x + 1) * indexY) + indexX;

			int increment = size.x + 1;

			indices.push_back(currentIndex);
			indices.push_back(currentIndex + 1);
			indices.push_back(currentIndex + increment);
			
			indices.push_back(currentIndex + increment);
			indices.push_back(currentIndex + 1);
			indices.push_back(currentIndex + increment + 1);			
		}
	}

	m_TotalIndices = indices.size();
	
	/* End algorithm to create vertices */
	glGenVertexArrays(1, &m_Vao);
	glBindVertexArray(m_Vao);

	GLuint vbo;
	GLuint ibo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexInfo), vertices.data(), GL_STATIC_DRAW);
	
	glBindVertexBuffer(0, vbo, 0, sizeof(VertexInfo));

	// Position will always go to location 0
	glEnableVertexAttribArray(0);
	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(VertexInfo, position));
	glVertexAttribBinding(0, 0);

	// UV will always go to location 1
	glEnableVertexAttribArray(1);
	glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, offsetof(VertexInfo, uv));
	glVertexAttribBinding(1, 0);
	
	// EBO
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

	glBindVertexArray(0);
}

//---------------------------------------------------------------------------------------------------------------------
void Grid::DrawMesh() const
{
	// Bind VAO
	glBindVertexArray(m_Vao);

	// Draw Triangles depending on the size
	glDrawElements(
		GL_TRIANGLES,      // mode
		m_TotalIndices,         // count
		GL_UNSIGNED_INT,   // type
		NULL               // element array buffer offset
	);

	// Unbind VAO
	glBindVertexArray(0);
}

//---------------------------------------------------------------------------------------------------------------------
const glm::uvec2& Grid::GetSize() const
{
	return m_Size;
}
