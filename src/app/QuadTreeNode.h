#pragma once
#include <graphic/Transform.h>
#include <graphic/BoundingBox.h>
#include <vector>

class ShaderProgram;
class Grid;

class QuadTreeNode
{
public:
	QuadTreeNode(
		const BoundingBox& boundingBox, 
		const int nodeLevel, 
		const glm::vec4& uvLimits,
		const std::vector<glm::vec2>& levelConfig, 
		Grid* grid);

	void Draw(ShaderProgram& shaderProgram, const glm::mat4& viewMatrix, const glm::vec3& cameraPosition);

private:
	void DrawNode(ShaderProgram& shaderProgram);

private:
	BoundingBox m_BoundingBox;
	int m_CurrentLevel;
	glm::vec2 m_MinMaxDistances; // x is Min, y is Max
	glm::vec4 m_UvLimits;
	Grid* m_Grid; // Grid
	Transform m_Transform;
	QuadTreeNode* m_Children[4];
};
