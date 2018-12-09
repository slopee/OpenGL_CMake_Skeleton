#include "QuadTree.h"
#include "QuadTreeNode.h"
#include <graphic/ShaderProgram.h>
#include <graphic/Camera.h>

//---------------------------------------------------------------------------------------------------------------------
QuadTree::QuadTree(const BoundingBox& boundingBox, const glm::vec2& distanceConfig, const glm::mat4& quadTransform, Grid* grid) :
	Renderable(
		Shader(SHADER_DIR"/quadtree.vert", GL_VERTEX_SHADER),
		Shader(SHADER_DIR"/quadtree.frag", GL_FRAGMENT_SHADER)),
	m_DistanceConfig(distanceConfig),
	m_QuadTransform(quadTransform)	
{
	m_LevelsConfig.push_back(LevelConfig(0.0f, 0.35f));
	m_LevelsConfig.push_back(LevelConfig(0.35f, 0.60f));
	m_LevelsConfig.push_back(LevelConfig(0.60f, 0.75f));
	m_LevelsConfig.push_back(LevelConfig(0.75f, 1.0f));
		
	std::vector<glm::vec2> finalDistances(m_LevelsConfig.size());	
	const float distDifference = distanceConfig.y - distanceConfig.x;
	for(int i = m_LevelsConfig.size() - 1, j = 0; i >= 0; --i, ++j)
	{		
		finalDistances[j].x = distanceConfig.x + (m_LevelsConfig[i].min * distDifference);
		finalDistances[j].y = distanceConfig.x + (m_LevelsConfig[i].max * distDifference);
	}

	m_RootNode = new QuadTreeNode(boundingBox, 0, finalDistances, grid);
}

//---------------------------------------------------------------------------------------------------------------------
void QuadTree::OnDraw(float time, const Camera& camera)
{
	m_ShaderProgram.setUniform("treeTransformation", m_QuadTransform);	
	m_RootNode->Draw(m_ShaderProgram, camera.GetViewMatrix(), camera.GetWorldPosition());
}
