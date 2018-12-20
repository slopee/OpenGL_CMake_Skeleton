#pragma once
#include <GL/glew.h>
#include <glm/detail/type_vec3.hpp>
#include <vector>
#include <glm/detail/type_vec2.hpp>
#include <glm/detail/type_vec4.hpp>
#include <unordered_map>

class QuadTreePatchMesh
{
public:
	enum Junction
	{
		Complete,
		Top,
		Right,
		Bottom,
		Left,
		TopRight,
		RightBottom,
		BottomLeft,
		TopLeft,

		Count
	};

	struct InstanceData
	{
		glm::vec3 scale;
		glm::vec3 color;
		glm::vec3 translation;
		glm::vec4 uvLimits;
	};

	QuadTreePatchMesh(float size);
	void DrawMesh(Junction junction, const std::vector<InstanceData>& instances) const;
	float GetPatchSize() const;

private:
	struct VertexInfo
	{
		glm::vec3 position;
		glm::vec2 uv;

		VertexInfo(glm::vec3 pos, glm::vec2 uv);
	};

private:
	void GenerateVao(Junction junction, const std::vector<VertexInfo>& vertices, const std::vector<GLuint>& indices);

	void GenerateFullPatchVao(float size, const std::vector<VertexInfo>& vertices);
	void GenerateTopVao(float size, const std::vector<VertexInfo>& vertices);
	void GenerateRightVao(float size, const std::vector<VertexInfo>& vertices);
	void GenerateBottomVao(float size, const std::vector<VertexInfo>& vertices);
	void GenerateLeftVao(float size, const std::vector<VertexInfo>& vertices);
	void GenerateTopRightVao(float size, const std::vector<VertexInfo>& vertices);
	void GenerateRightBottomVao(float size, const std::vector<VertexInfo>& vertices);
	void GenerateBottomLeftVao(float size, const std::vector<VertexInfo>& vertices);
	void GenerateTopLeftVao(float size, const std::vector<VertexInfo>& vertices);

	void AddGridPatchIndices(int currentIndex, int increment, std::vector<GLuint>& indices);

private:
	float m_Size;
	GLuint m_Vbo;
	GLuint m_Vaos[Junction::Count];
	GLuint m_InstanceVbo[Junction::Count];
	int m_TotalIndices[Junction::Count];
};
