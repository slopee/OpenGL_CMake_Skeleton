#include "QuadTreeNode.h"
#include <graphic/BoundingBox.h>
#include <graphic/ShaderProgram.h>
#include <glm/gtc/matrix_transform.inl>
#include <graphic/Camera.h>
#include <app/QuadTreePatchMesh.h>
#include <unordered_set>

static int nextId = 0;

QuadTreeNode::QuadTreeNode(
	const BoundingBox& boundingBox,
	const int nodeLevel,
	const glm::vec4& uvLimits,
	const std::vector<glm::vec2>& levelConfig,
	std::shared_ptr<QuadTreePatchMesh> quadTreePatchMesh,
	QuadTreeNode* parent,
	std::unordered_map<int, std::weak_ptr<QuadTreeNode>>& nodesMap) :
	m_Id(++nextId),
	m_BoundingBox(boundingBox),
	m_CurrentLevel(nodeLevel),
	m_MinMaxDistances(levelConfig[nodeLevel]),
	m_UvLimits(uvLimits),
	m_Parent(parent),
	m_PatchMesh(quadTreePatchMesh),
	m_Visited(false)
{
	const auto center = boundingBox.GetCenter();
	m_Transform.position = glm::vec3(center.x, center.y, 0.0f);

	m_Children[0] = nullptr;
	m_Children[1] = nullptr;
	m_Children[2] = nullptr;
	m_Children[3] = nullptr;

	if (nodeLevel + 1 < levelConfig.size())
	{
		const auto boundingWidth = boundingBox.GetWidth() / 2.0f;
		const auto boundingHeight = boundingBox.GetHeight() / 2.0f;

		const auto halfUv = glm::vec2(uvLimits.x + ((uvLimits.z - uvLimits.x) / 2.0f),
		                              uvLimits.y + ((uvLimits.w - uvLimits.y) / 2.0f));

		const BoundingBox bottomLeftBoundingBox(center.x - boundingWidth, center.y - boundingHeight, boundingWidth,
		                                        boundingHeight);
		const glm::vec4 bottomLeftUvLimits(uvLimits.x, uvLimits.y, halfUv.x, halfUv.y);
		m_Children[0] = std::make_shared<QuadTreeNode>(bottomLeftBoundingBox, nodeLevel + 1, bottomLeftUvLimits, levelConfig,
		                                               quadTreePatchMesh, this, nodesMap);

		const BoundingBox bottomRightBoundingBox(center.x, center.y - boundingHeight, boundingWidth, boundingHeight);
		const glm::vec4 bottomRightUvLimits(halfUv.x, uvLimits.y, uvLimits.z, halfUv.y);
		m_Children[1] = std::make_shared<QuadTreeNode>(bottomRightBoundingBox, nodeLevel + 1, bottomRightUvLimits,
		                                               levelConfig, quadTreePatchMesh, this, nodesMap);

		const BoundingBox topLeftBoundingBox(center.x - boundingWidth, center.y, boundingWidth, boundingHeight);
		const glm::vec4 topLeftUvLimits(uvLimits.x, halfUv.y, halfUv.x, uvLimits.w);
		m_Children[2] = std::make_shared<QuadTreeNode>(topLeftBoundingBox, nodeLevel + 1, topLeftUvLimits, levelConfig,
		                                               quadTreePatchMesh, this, nodesMap);

		const BoundingBox topRightBoundingBox(center.x, center.y, boundingWidth, boundingHeight);
		const glm::vec4 topRightUvLimits(halfUv.x, halfUv.y, uvLimits.z, uvLimits.w);
		m_Children[3] = std::make_shared<QuadTreeNode>(topRightBoundingBox, nodeLevel + 1, topRightUvLimits, levelConfig,
		                                               quadTreePatchMesh, this, nodesMap);

		// Set Neighbours
		m_Children[1]->AddNeighbour(m_Children[0], Left);
		m_Children[3]->AddNeighbour(m_Children[2], Left);
		m_Children[0]->AddNeighbour(m_Children[1], Right);
		m_Children[2]->AddNeighbour(m_Children[3], Right);
		m_Children[3]->AddNeighbour(m_Children[1], Bottom);
		m_Children[2]->AddNeighbour(m_Children[0], Bottom);
		m_Children[1]->AddNeighbour(m_Children[3], Top);
		m_Children[0]->AddNeighbour(m_Children[2], Top);

		for(auto children : m_Children)
		{
			nodesMap.insert({ children->m_Id, children });
		}		
	}
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void QuadTreeNode::ResetNodes()
{
	m_Visited = false;
	m_WillBeDrawn = false;
	for(auto child : m_Children)
	{
		if(child != nullptr)
		{
			child->ResetNodes();
		}		
	}
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void QuadTreeNode::SelectNodesToDraw(std::vector<std::unordered_set<int>>& nodes,
                                     const Camera& camera,
                                     const glm::mat4& quadTransform,
                                     const int minLodLevel)
{
	if(m_Visited)
	{
		if(m_CurrentLevel >= minLodLevel)
		{
			return;
		}

		auto& currentNodesLodLevel = nodes[m_CurrentLevel];		
		currentNodesLodLevel.erase(m_Id);
		m_WillBeDrawn = false;
	}
	
	m_Visited = true;

	const float cameraDistance = GetClosestPointToCamera(camera, quadTransform);

	// We expand this node if the camera is less than the one we want or we are forcing the LOD level		
	if (cameraDistance < m_MinMaxDistances.x || m_CurrentLevel < minLodLevel)
	{
		for (int i = 0; i < 4; ++i)
		{
			if (m_Children[i] != nullptr)
			{
				m_Children[i]->SelectNodesToDraw(nodes, camera, quadTransform, minLodLevel);
			}
		}
	}
	else if (cameraDistance <= m_MinMaxDistances.y || m_CurrentLevel > 0)
	{
		nodes[m_CurrentLevel].insert(m_Id);
		m_WillBeDrawn = true;

		// Check if neighbour should or not be added		
		ForceLodDifferenceNeighbour(Top, nodes, camera ,quadTransform);
		ForceLodDifferenceNeighbour(Right, nodes, camera, quadTransform);
		ForceLodDifferenceNeighbour(Bottom, nodes, camera, quadTransform);
		ForceLodDifferenceNeighbour(Left, nodes, camera, quadTransform);
	}
}

//-----------------------------------------------------------------------------------------------------------------------------------------
std::tuple<QuadTreePatchMesh::Junction, QuadTreePatchMesh::InstanceData> QuadTreeNode::GetNodeDrawData() const
{
	static glm::vec3 colors[4]{
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.5f, 0.5f, 0.0f)
	};
	const auto patchSize = m_PatchMesh->GetPatchSize();
	const auto scale = glm::vec3(m_BoundingBox.GetWidth() / patchSize, m_BoundingBox.GetHeight() / patchSize, 1.0f);

	return std::make_tuple(
		GetMeshJunctionType(), 
		QuadTreePatchMesh::InstanceData { 
			scale, 
			colors[m_CurrentLevel], 
			m_Transform.position, 
			m_UvLimits 
		});
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void QuadTreeNode::ForceLodDifferenceNeighbour(
	NeighbourPosition neighbourPosition,
	std::vector<std::unordered_set<int>>& nodes,
	const Camera& camera,
	const glm::mat4& quadTransform)
{	
	if (m_Neighbours[neighbourPosition].expired())
	{
		return;
	}

	auto neighbour = m_Neighbours[neighbourPosition].lock();
	if (neighbour->m_Parent->m_Id == m_Parent->m_Id)
	{
		return;
	}
	
	if (!neighbour->m_Visited && !neighbour->m_Parent->m_Visited)
	{
		int lodDifference = 1;
		auto* neighbourParent = neighbour->m_Parent;
		while (neighbourParent != nullptr && !neighbourParent->m_Visited)
		{
			++lodDifference;
			neighbourParent = neighbourParent->m_Parent;
		}

		if (lodDifference > 1 && neighbourParent->m_Id != 1)
		{
			neighbourParent->SelectNodesToDraw(nodes, camera, quadTransform, m_CurrentLevel - 1);
		}
	}
}

//-----------------------------------------------------------------------------------------------------------------------------------------
void QuadTreeNode::AddNeighbour(std::weak_ptr<QuadTreeNode> neighbour, NeighbourPosition position)
{
	m_Neighbours[position] = neighbour;
	if (m_Children[0] == nullptr)
	{
		return;
	}

	const auto& neighbourChildren = neighbour.lock()->m_Children;
	if (position == Left)
	{
		m_Children[0]->AddNeighbour(neighbourChildren[1], Left);
		m_Children[2]->AddNeighbour(neighbourChildren[3], Left);
	}
	else if (position == Right)
	{
		m_Children[1]->AddNeighbour(neighbourChildren[0], Right);
		m_Children[3]->AddNeighbour(neighbourChildren[2], Right);
	}
	else if (position == Bottom)
	{
		m_Children[0]->AddNeighbour(neighbourChildren[2], Bottom);
		m_Children[1]->AddNeighbour(neighbourChildren[3], Bottom);
	}
	else if (position == Top)
	{
		m_Children[2]->AddNeighbour(neighbourChildren[0], Top);
		m_Children[3]->AddNeighbour(neighbourChildren[1], Top);
	}
}

//-----------------------------------------------------------------------------------------------------------------------------------------
float QuadTreeNode::GetClosestPointToCamera(const Camera& camera, const glm::mat4& quadTransform) const
{
	const auto& origin = m_BoundingBox.GetOrigin();
	const auto width = m_BoundingBox.GetWidth();
	const auto height = m_BoundingBox.GetHeight();
	const std::vector<glm::vec4> nodePoints{
		glm::vec4(origin.x, origin.y, 0.0f, 1.0f),
		glm::vec4(origin.x + width, origin.y, 0.0f, 1.0f),
		glm::vec4(origin.x, origin.y + height, 0.0f, 1.0f),
		glm::vec4(origin.x + width, origin.y + height, 0.0f, 1.0f)
	};

	float minDistance = std::numeric_limits<float>::infinity();
	for (auto& point : nodePoints)
	{
		const auto worldPosition = quadTransform * point;
		const float cameraDistance = glm::distance(camera.GetWorldPosition(), glm::vec3(worldPosition));
		if (cameraDistance < minDistance)
		{
			minDistance = cameraDistance;
		}
	}

	return minDistance;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
QuadTreePatchMesh::Junction QuadTreeNode::GetMeshJunctionType() const
{
	const bool topHasLessLod = HasNeighbourLessLod(Top);
	const bool rightHasLessLod = HasNeighbourLessLod(Right);
	const bool bottomHasLessLod = HasNeighbourLessLod(Bottom);
	const bool leftHasLessLod = HasNeighbourLessLod(Left);

	auto meshTypeToDraw = QuadTreePatchMesh::Junction::Complete;
	if (topHasLessLod)
	{
		if (rightHasLessLod)
		{
			meshTypeToDraw = QuadTreePatchMesh::TopRight;
		}
		else if (leftHasLessLod)
		{
			meshTypeToDraw = QuadTreePatchMesh::TopLeft;
		}
		else
		{
			meshTypeToDraw = QuadTreePatchMesh::Top;
		}
	}
	else if(bottomHasLessLod)
	{
		if (rightHasLessLod)
		{
			meshTypeToDraw = QuadTreePatchMesh::RightBottom;
		}
		else if (leftHasLessLod)
		{
			meshTypeToDraw = QuadTreePatchMesh::BottomLeft;
		}
		else
		{
			meshTypeToDraw = QuadTreePatchMesh::Bottom;
		}
	}
	else if(rightHasLessLod)
	{
		meshTypeToDraw = QuadTreePatchMesh::Right;
	}
	else if(leftHasLessLod)
	{
		meshTypeToDraw = QuadTreePatchMesh::Left;
	}

	return meshTypeToDraw;
}

//-----------------------------------------------------------------------------------------------------------------------------------------
bool QuadTreeNode::HasNeighbourLessLod(NeighbourPosition neighbourPosition) const
{
	if(m_Neighbours[neighbourPosition].expired())
	{
		return false;
	}

	auto neighbour = m_Neighbours[neighbourPosition].lock();
	return !neighbour->m_Visited || 
		(neighbour->m_Visited && !neighbour->m_WillBeDrawn && neighbour->m_Parent != nullptr && neighbour->m_Parent->m_WillBeDrawn);
}
