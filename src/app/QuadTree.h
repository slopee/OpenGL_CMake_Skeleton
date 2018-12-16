#pragma once
#include <glm/detail/type_vec2.hpp>
#include <vector>
#include <glm/detail/type_mat4x4.hpp>
#include "graphic/Renderable.h"
#include "QuadTreePatchMesh.h"
#include <memory>
#include <unordered_map>

class ShaderProgram;
class Grid;
class QuadTreeNode;
class BoundingBox;

class QuadTree : public Renderable
{
public:
	struct LevelConfig
	{
		float min;
		float max;
		
		LevelConfig(float min, float max)
			: min(min),
			  max(max)
		{
		}
	};

public:
	QuadTree(const BoundingBox& boundingBox, const glm::vec2& distanceConfig, const glm::mat4& quadTransform, Grid* grid);
	//void Draw(float time, ShaderProgram& shaderProgram, const glm::mat4& viewMatrix, const glm::vec3& cameraPosition);

protected:
	void OnDraw(float time, const Camera& camera) override;

private:
	glm::vec2 m_DistanceConfig;
	glm::mat4 m_QuadTransform;	
	std::vector<LevelConfig> m_LevelsConfig;
	std::unique_ptr<QuadTreeNode> m_RootNode;	
	std::shared_ptr<QuadTreePatchMesh> m_PatchMesh;
	std::unordered_map<int, std::weak_ptr<QuadTreeNode>> m_Nodes;
};
