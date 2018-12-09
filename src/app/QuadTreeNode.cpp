#include "QuadTreeNode.h"
#include <graphic/BoundingBox.h>
#include <graphic/ShaderProgram.h>
#include <app/Grid.h>
#include <glm/gtc/matrix_transform.inl>

QuadTreeNode::QuadTreeNode(
	const BoundingBox& boundingBox, 
	const int nodeLevel, 
	const glm::vec4& uvLimits,
	const std::vector<glm::vec2>& levelConfig, 
	Grid* grid) :
	m_BoundingBox(boundingBox),
	m_CurrentLevel(nodeLevel),
	m_MinMaxDistances(levelConfig[nodeLevel]),
	m_UvLimits(uvLimits),
	m_Grid(grid)
{
	const auto center = boundingBox.GetCenter();
	m_Transform.position = glm::vec3(center.x, center.y, 0.0f);

	m_Children[0] = nullptr;
	m_Children[1] = nullptr;
	m_Children[2] = nullptr;
	m_Children[3] = nullptr;

	if(nodeLevel + 1 < levelConfig.size())
	{
		const auto boundingWidth = boundingBox.GetWidth() / 2.0f;
		const auto boundingHeight = boundingBox.GetHeight() / 2.0f;

		const auto halfUv = glm::vec2(uvLimits.x + ((uvLimits.z - uvLimits.x) / 2.0f), uvLimits.y + ((uvLimits.w - uvLimits.y) / 2.0f));
		
		const BoundingBox bottomLeftBoundingBox(center.x - boundingWidth, center.y - boundingHeight, boundingWidth, boundingHeight);
		const glm::vec4 bottomLeftUvLimits(uvLimits.x, uvLimits.y, halfUv.x, halfUv.y);
		m_Children[0] = new QuadTreeNode(bottomLeftBoundingBox, nodeLevel + 1, bottomLeftUvLimits, levelConfig, grid);

		const BoundingBox topLeftBoundingBox(center.x - boundingWidth, center.y, boundingWidth, boundingHeight);
		const glm::vec4 topLeftUvLimits(uvLimits.x, halfUv.y, halfUv.x, uvLimits.w);
		m_Children[1] = new QuadTreeNode(topLeftBoundingBox, nodeLevel + 1, topLeftUvLimits, levelConfig, grid);
				
		const BoundingBox bottomRightBoundingBox(center.x, center.y - boundingHeight, boundingWidth, boundingHeight);
		const glm::vec4 bottomRightUvLimits(halfUv.x, uvLimits.y, uvLimits.z, halfUv.y);
		m_Children[2] = new QuadTreeNode(bottomRightBoundingBox, nodeLevel + 1, bottomRightUvLimits, levelConfig, grid);
		
		const BoundingBox topRightBoundingBox(center.x, center.y, boundingWidth, boundingHeight);
		const glm::vec4 topRightUvLimits(halfUv.x, halfUv.y, uvLimits.z, uvLimits.w);
		m_Children[3] = new QuadTreeNode(topRightBoundingBox, nodeLevel + 1, topRightUvLimits, levelConfig, grid);
	}
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void QuadTreeNode::Draw(ShaderProgram& shaderProgram, const glm::mat4& viewMatrix, const glm::vec3& cameraPosition)
{
	//const auto boundingCenterWorldPosition = glm::vec3(viewMatrix * glm::vec4(m_Transform.position, 1.0f));		
	const float cameraDistance = glm::distance(cameraPosition, m_Transform.position);

	// We expand this node if the camera is less than the one we want
	if (cameraDistance < m_MinMaxDistances.x)
	{
		for (int i = 0; i < 4; ++i)
		{
			if (m_Children[i] != nullptr)
			{
				m_Children[i]->Draw(shaderProgram, viewMatrix, cameraPosition);
			}
		}
	}
	else if(cameraDistance <= m_MinMaxDistances.y || m_CurrentLevel > 0)
	{		
		DrawNode(shaderProgram);
	}		
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void QuadTreeNode::DrawNode(ShaderProgram& shaderProgram)
{
	static glm::vec3 colors[4]{
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.5f, 0.5f, 0.0f)
	};

	const auto gridSize = m_Grid->GetSize();
	glm::mat4 gridScale = glm::scale(glm::mat4(), glm::vec3(m_BoundingBox.GetWidth() / gridSize.x, m_BoundingBox.GetHeight() / gridSize.y, 1.0f));

	shaderProgram.setUniform("gridScale", gridScale);
	shaderProgram.setUniform("nodeColor", colors[m_CurrentLevel]);
	shaderProgram.setUniform("nodeTransformation", m_Transform.GetTransformMatrix());
	shaderProgram.setUniform("uvLimits", m_UvLimits);
	m_Grid->DrawMesh();
}
