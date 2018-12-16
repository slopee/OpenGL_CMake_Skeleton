#pragma once
#include <graphic/Transform.h>
#include <graphic/BoundingBox.h>
#include <vector>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include "QuadTreePatchMesh.h"

class Camera;
class ShaderProgram;
class Grid;

class QuadTreeNode : public std::enable_shared_from_this<QuadTreeNode>
{
public:
	QuadTreeNode(
		const BoundingBox& boundingBox, 
		const int nodeLevel, 
		const glm::vec4& uvLimits,
		const std::vector<glm::vec2>& levelConfig, 
		std::shared_ptr<QuadTreePatchMesh> quadTreePatchMesh,
		QuadTreeNode* parent,
		std::unordered_map<int, std::weak_ptr<QuadTreeNode>>& nodesMap);

	void ResetNodes();
	void SelectNodesToDraw(
		std::vector<std::unordered_set<int>>& nodes, 
		const Camera& camera, 
		const glm::mat4& quadTransform,
		const int maxLodLevel);

	void DrawNode(ShaderProgram& shaderProgram) const;
	

private:
	enum NeighbourPosition
	{
		Top,		
		Right,
		Bottom,
		Left
	};

private:
	void ForceLodDifferenceNeighbour(
		NeighbourPosition neighbourPosition,
		std::vector<std::unordered_set<int>>& nodes,
		const Camera& camera,
		const glm::mat4& quadTransform);	
	void AddNeighbour(std::weak_ptr<QuadTreeNode> neighbour, NeighbourPosition position);
	float GetClosestPointToCamera(const Camera& camera, const glm::mat4& quadTransform) const;
	QuadTreePatchMesh::Junction GetMeshJunctionType() const;
	bool HasNeighbourLessLod(NeighbourPosition neighbourPosition) const;

private:
	int m_Id;
	BoundingBox m_BoundingBox;
	int m_CurrentLevel;
	glm::vec2 m_MinMaxDistances; // x is Min, y is Max
	glm::vec4 m_UvLimits;
	
	Transform m_Transform;
	QuadTreeNode* m_Parent;
	std::shared_ptr<QuadTreeNode> m_Children[4];
	std::shared_ptr<QuadTreePatchMesh> m_PatchMesh;
	std::weak_ptr<QuadTreeNode> m_Neighbours[4];

	bool m_Visited;
	bool m_WillBeDrawn;
};
