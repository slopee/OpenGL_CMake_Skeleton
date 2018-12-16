#include "QuadTreePatchMesh.h"

//-----------------------------------------------------------------------------------------------------------------------------------------
QuadTreePatchMesh::QuadTreePatchMesh(float size) : m_Size(size), m_Vbo(-1)
{
	int totalVerticesSize = (size + 1) * (size + 1);
	glm::vec2 halfSize(size / 2.0f, size / 2.0f);

	std::vector<VertexInfo> vertices;
	vertices.reserve(totalVerticesSize);

	std::vector<GLuint> indices;
	indices.reserve(totalVerticesSize * 4);

	/* Init algorithm to create vertices */
	float currentY = 0.0f;
	float currentX = 0.0f;
	int verticesPerRow = 0;
	for (int i = 0; i < totalVerticesSize; ++i)
	{
		vertices.push_back(VertexInfo(
			glm::vec3(currentX - halfSize.x, currentY - halfSize.y, 0.0f),
			glm::vec2(currentX / size, currentY / size)));
		++verticesPerRow;

		if (verticesPerRow == (size + 1))
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

	// Generate each Vao according to each patch
	GenerateFullPatchVao(size, vertices);
	GenerateTopVao(size, vertices);
	GenerateRightVao(size, vertices);
	GenerateBottomVao(size, vertices);
	GenerateLeftVao(size, vertices);
	GenerateTopRightVao(size, vertices);
	GenerateRightBottomVao(size, vertices);
	GenerateBottomLeftVao(size, vertices);
	GenerateTopLeftVao(size, vertices);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void QuadTreePatchMesh::DrawMesh(Junction junction) const
{
	if (junction < Junction::Count)
	{
		glBindVertexArray(m_Vaos[junction]);
		glDrawElements(GL_TRIANGLES, m_TotalIndices[junction], GL_UNSIGNED_INT, NULL);
		glBindVertexArray(0);
	}
}

//-----------------------------------------------------------------------------------------------------------------------------------------
float QuadTreePatchMesh::GetPatchSize() const
{
	return m_Size;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
QuadTreePatchMesh::VertexInfo::VertexInfo(glm::vec3 pos, glm::vec2 uv) : position(pos), uv(uv)
{
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void QuadTreePatchMesh::GenerateVao(int index, const std::vector<VertexInfo>& vertices,
                                    const std::vector<GLuint>& indices)
{
	glGenVertexArrays(1, &m_Vaos[index]);
	glBindVertexArray(m_Vaos[index]);

	if (m_Vbo == -1)
	{
		glGenBuffers(1, &m_Vbo);
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexInfo), vertices.data(), GL_STATIC_DRAW);
	}
	else
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_Vbo);
	}

	glBindVertexBuffer(0, m_Vbo, 0, sizeof(VertexInfo));

	// Position will always go to location 0
	glEnableVertexAttribArray(0);
	glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, offsetof(VertexInfo, position));
	glVertexAttribBinding(0, 0);

	// UV will always go to location 1
	glEnableVertexAttribArray(1);
	glVertexAttribFormat(1, 2, GL_FLOAT, GL_FALSE, offsetof(VertexInfo, uv));
	glVertexAttribBinding(1, 0);

	// EBO
	GLuint ibo;
	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
	m_TotalIndices[index] = indices.size();

	glBindVertexArray(0);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void QuadTreePatchMesh::GenerateFullPatchVao(float size, const std::vector<VertexInfo>& vertices)
{
	std::vector<GLuint> indices;
	indices.reserve(vertices.size() * 4);

	for (int indexY = 0; indexY < size; ++indexY)
	{
		for (int indexX = 0; indexX < size; ++indexX)
		{
			int currentIndex = ((size + 1) * indexY) + indexX;
			int rowIncrement = size + 1;

			AddGridPatchIndices(currentIndex, rowIncrement, indices);
		}
	}

	GenerateVao(Junction::Complete, vertices, indices);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void QuadTreePatchMesh::GenerateTopVao(float size, const std::vector<VertexInfo>& vertices)
{
	std::vector<GLuint> indices;
	indices.reserve(vertices.size() * 4);

	for (int indexY = 0; indexY < size; ++indexY)
	{
		for (int indexX = 0; indexX < size; ++indexX)
		{
			const int currentIndex = ((size + 1) * indexY) + indexX;
			const int rowrowIncrement = size + 1;

			if (indexY == size - 1 && indexX % 2 == 1)
			{
				indices.push_back(currentIndex - 1);
				indices.push_back(currentIndex);
				indices.push_back(currentIndex - 1 + rowrowIncrement);

				indices.push_back(currentIndex);
				indices.push_back(currentIndex + 1);
				indices.push_back(currentIndex + rowrowIncrement + 1);

				indices.push_back(currentIndex - 1 + rowrowIncrement);
				indices.push_back(currentIndex);
				indices.push_back(currentIndex + 1 + rowrowIncrement);
			}
			else if (indexY != size - 1)
			{
				AddGridPatchIndices(currentIndex, rowrowIncrement, indices);
			}
		}
	}

	GenerateVao(Junction::Top, vertices, indices);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void QuadTreePatchMesh::GenerateRightVao(float size, const std::vector<VertexInfo>& vertices)
{
	std::vector<GLuint> indices;
	indices.reserve(vertices.size() * 4);

	for (int indexY = 0; indexY < size; ++indexY)
	{
		for (int indexX = 0; indexX < size; ++indexX)
		{
			const int currentIndex = ((size + 1) * indexY) + indexX;
			const int rowIncrement = size + 1;

			if (indexX == size - 1 && indexY % 2 == 0)
			{
				indices.push_back(currentIndex);
				indices.push_back(currentIndex + 1);
				indices.push_back(currentIndex + rowIncrement);

				indices.push_back(currentIndex + rowIncrement);
				indices.push_back(currentIndex + rowIncrement + rowIncrement + 1);
				indices.push_back(currentIndex + rowIncrement + rowIncrement);

				indices.push_back(currentIndex + 1);
				indices.push_back(currentIndex + rowIncrement + rowIncrement + 1);
				indices.push_back(currentIndex + rowIncrement);
			}
			else if (indexX != size - 1)
			{
				AddGridPatchIndices(currentIndex, rowIncrement, indices);
			}
		}
	}

	GenerateVao(Junction::Right, vertices, indices);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void QuadTreePatchMesh::GenerateBottomVao(float size, const std::vector<VertexInfo>& vertices)
{
	std::vector<GLuint> indices;
	indices.reserve(vertices.size() * 4);

	for (int indexY = 0; indexY < size; ++indexY)
	{
		for (int indexX = 0; indexX < size; ++indexX)
		{
			const int currentIndex = ((size + 1) * indexY) + indexX;
			const int rowrowIncrement = size + 1;

			if (indexY == 0 && indexX % 2 == 0)
			{
				indices.push_back(currentIndex);
				indices.push_back(currentIndex + rowrowIncrement + 1);
				indices.push_back(currentIndex + rowrowIncrement);

				indices.push_back(currentIndex + rowrowIncrement + 1);
				indices.push_back(currentIndex + 2);
				indices.push_back(currentIndex + rowrowIncrement + 2);

				indices.push_back(currentIndex);
				indices.push_back(currentIndex + 2);
				indices.push_back(currentIndex + rowrowIncrement + 1);
			}
			else if (indexY != 0)
			{
				AddGridPatchIndices(currentIndex, rowrowIncrement, indices);
			}
		}
	}

	GenerateVao(Junction::Bottom, vertices, indices);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void QuadTreePatchMesh::GenerateLeftVao(float size, const std::vector<VertexInfo>& vertices)
{
	std::vector<GLuint> indices;
	indices.reserve(vertices.size() * 4);

	for (int indexY = 0; indexY < size; ++indexY)
	{
		for (int indexX = 0; indexX < size; ++indexX)
		{
			const int currentIndex = ((size + 1) * indexY) + indexX;
			const int rowIncrement = size + 1;

			if (indexX == 0 && indexY % 2 == 0)
			{
				indices.push_back(currentIndex);
				indices.push_back(currentIndex + 1);
				indices.push_back(currentIndex + rowIncrement + 1);

				indices.push_back(currentIndex + rowIncrement + 1);
				indices.push_back(currentIndex + rowIncrement + rowIncrement + 1);
				indices.push_back(currentIndex + rowIncrement + rowIncrement);

				indices.push_back(currentIndex);
				indices.push_back(currentIndex + rowIncrement + 1);
				indices.push_back(currentIndex + rowIncrement + rowIncrement);
			}
			else if (indexX != 0)
			{
				AddGridPatchIndices(currentIndex, rowIncrement, indices);
			}
		}
	}

	GenerateVao(Junction::Left, vertices, indices);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void QuadTreePatchMesh::GenerateTopRightVao(float size, const std::vector<VertexInfo>& vertices)
{
	std::vector<GLuint> indices;
	indices.reserve(vertices.size() * 4);

	for (int indexY = 0; indexY < size; ++indexY)
	{
		for (int indexX = 0; indexX < size; ++indexX)
		{
			const int currentIndex = ((size + 1) * indexY) + indexX;
			const int rowrowIncrement = size + 1;

			// TOP
			if (indexY == size - 1 && indexX % 2 == 1)
			{
				indices.push_back(currentIndex - 1);
				indices.push_back(currentIndex);
				indices.push_back(currentIndex - 1 + rowrowIncrement);

				if (indexX == 1)
				{
					indices.push_back(currentIndex);
					indices.push_back(currentIndex + 1);
					indices.push_back(currentIndex + rowrowIncrement + 1);
				}

				indices.push_back(currentIndex - 1 + rowrowIncrement);
				indices.push_back(currentIndex);
				indices.push_back(currentIndex + 1 + rowrowIncrement);
			}
			else if (indexX == size - 1 && indexY % 2 == 0) // Right
			{
				indices.push_back(currentIndex);
				indices.push_back(currentIndex + 1);
				indices.push_back(currentIndex + rowrowIncrement);

				if (indexY == 0)
				{
					indices.push_back(currentIndex + rowrowIncrement);
					indices.push_back(currentIndex + rowrowIncrement + rowrowIncrement + 1);
					indices.push_back(currentIndex + rowrowIncrement + rowrowIncrement);
				}

				indices.push_back(currentIndex + 1);
				indices.push_back(currentIndex + rowrowIncrement + rowrowIncrement + 1);
				indices.push_back(currentIndex + rowrowIncrement);
			}
			else if (indexY != size - 1 && indexX != size - 1)
			{
				AddGridPatchIndices(currentIndex, rowrowIncrement, indices);
			}
		}
	}

	GenerateVao(Junction::TopRight, vertices, indices);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void QuadTreePatchMesh::GenerateRightBottomVao(float size, const std::vector<VertexInfo>& vertices)
{
	std::vector<GLuint> indices;
	indices.reserve(vertices.size() * 4);

	for (int indexY = 0; indexY < size; ++indexY)
	{
		for (int indexX = 0; indexX < size; ++indexX)
		{
			const int currentIndex = ((size + 1) * indexY) + indexX;
			const int rowIncrement = size + 1;

			if (indexY == 0 && indexX % 2 == 0) // Bottom
			{
				indices.push_back(currentIndex);
				indices.push_back(currentIndex + rowIncrement + 1);
				indices.push_back(currentIndex + rowIncrement);

				if (indexX == 0)
				{
					indices.push_back(currentIndex + rowIncrement + 1);
					indices.push_back(currentIndex + 2);
					indices.push_back(currentIndex + rowIncrement + 2);
				}

				indices.push_back(currentIndex);
				indices.push_back(currentIndex + 2);
				indices.push_back(currentIndex + rowIncrement + 1);
			}
			else if (indexX == size - 1 && indexY % 2 == 0) // Right
			{
				if (indexY != 0)
				{
					indices.push_back(currentIndex);
					indices.push_back(currentIndex + 1);
					indices.push_back(currentIndex + rowIncrement);
				}

				indices.push_back(currentIndex + rowIncrement);
				indices.push_back(currentIndex + rowIncrement + rowIncrement + 1);
				indices.push_back(currentIndex + rowIncrement + rowIncrement);

				indices.push_back(currentIndex + 1);
				indices.push_back(currentIndex + rowIncrement + rowIncrement + 1);
				indices.push_back(currentIndex + rowIncrement);
			}
			else if (indexY != 0 && indexX != size - 1)
			{
				AddGridPatchIndices(currentIndex, rowIncrement, indices);
			}
		}
	}

	GenerateVao(Junction::RightBottom, vertices, indices);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void QuadTreePatchMesh::GenerateBottomLeftVao(float size, const std::vector<VertexInfo>& vertices)
{
	std::vector<GLuint> indices;
	indices.reserve(vertices.size() * 4);

	for (int indexY = 0; indexY < size; ++indexY)
	{
		for (int indexX = 0; indexX < size; ++indexX)
		{
			const int currentIndex = ((size + 1) * indexY) + indexX;
			const int rowIncrement = size + 1;

			if (indexY == 0 && indexX % 2 == 0) // Bottom
			{
				if (indexX != 0)
				{
					indices.push_back(currentIndex);
					indices.push_back(currentIndex + rowIncrement + 1);
					indices.push_back(currentIndex + rowIncrement);
				}

				indices.push_back(currentIndex + rowIncrement + 1);
				indices.push_back(currentIndex + 2);
				indices.push_back(currentIndex + rowIncrement + 2);

				indices.push_back(currentIndex);
				indices.push_back(currentIndex + 2);
				indices.push_back(currentIndex + rowIncrement + 1);
			}

			if (indexX == 0 && indexY % 2 == 0) // Left
			{
				if (indexY != 0)
				{
					indices.push_back(currentIndex);
					indices.push_back(currentIndex + 1);
					indices.push_back(currentIndex + rowIncrement + 1);
				}

				indices.push_back(currentIndex + rowIncrement + 1);
				indices.push_back(currentIndex + rowIncrement + rowIncrement + 1);
				indices.push_back(currentIndex + rowIncrement + rowIncrement);


				indices.push_back(currentIndex);
				indices.push_back(currentIndex + rowIncrement + 1);
				indices.push_back(currentIndex + rowIncrement + rowIncrement);
			}
			else if (indexY != 0 && indexX != 0)
			{
				AddGridPatchIndices(currentIndex, rowIncrement, indices);
			}
		}
	}

	GenerateVao(Junction::BottomLeft, vertices, indices);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void QuadTreePatchMesh::GenerateTopLeftVao(float size, const std::vector<VertexInfo>& vertices)
{
	std::vector<GLuint> indices;
	indices.reserve(vertices.size() * 4);

	for (int indexY = 0; indexY < size; ++indexY)
	{
		for (int indexX = 0; indexX < size; ++indexX)
		{
			const int currentIndex = ((size + 1) * indexY) + indexX;
			const int rowIncrement = size + 1;

			if (indexY == size - 1 && indexX % 2 == 1) // Top
			{
				if (indexX != 1)
				{
					indices.push_back(currentIndex - 1);
					indices.push_back(currentIndex);
					indices.push_back(currentIndex - 1 + rowIncrement);
				}

				indices.push_back(currentIndex);
				indices.push_back(currentIndex + 1);
				indices.push_back(currentIndex + rowIncrement + 1);

				indices.push_back(currentIndex - 1 + rowIncrement);
				indices.push_back(currentIndex);
				indices.push_back(currentIndex + 1 + rowIncrement);
			}
			else if (indexX == 0 && indexY % 2 == 0)
			{
				indices.push_back(currentIndex);
				indices.push_back(currentIndex + 1);
				indices.push_back(currentIndex + rowIncrement + 1);

				if (indexY == 0)
				{
					indices.push_back(currentIndex + rowIncrement + 1);
					indices.push_back(currentIndex + rowIncrement + rowIncrement + 1);
					indices.push_back(currentIndex + rowIncrement + rowIncrement);
				}

				indices.push_back(currentIndex);
				indices.push_back(currentIndex + rowIncrement + 1);
				indices.push_back(currentIndex + rowIncrement + rowIncrement);
			}
			else if (indexY != size - 1 && indexX != 0)
			{
				AddGridPatchIndices(currentIndex, rowIncrement, indices);
			}
		}
	}

	GenerateVao(Junction::TopLeft, vertices, indices);
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void QuadTreePatchMesh::AddGridPatchIndices(int currentIndex, int rowIncrement, std::vector<GLuint>& indices)
{
	indices.push_back(currentIndex);
	indices.push_back(currentIndex + 1);
	indices.push_back(currentIndex + rowIncrement);

	indices.push_back(currentIndex + rowIncrement);
	indices.push_back(currentIndex + 1);
	indices.push_back(currentIndex + rowIncrement + 1);
}
