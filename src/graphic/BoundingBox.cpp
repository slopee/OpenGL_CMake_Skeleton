#include "BoundingBox.h"
#include <glm/detail/type_vec2.hpp>

//---------------------------------------------------------------------------------------------------------------------
BoundingBox::BoundingBox(float x, float y, float width, float height) : 
	m_Bounds(x, y, x + width, y + height)
{
}

//---------------------------------------------------------------------------------------------------------------------
BoundingBox::BoundingBox(const glm::vec2& center, float width, float height) :
	m_Bounds(center.x - width / 2.0f, center.y - height / 2.0f, width, height)
{
}

//---------------------------------------------------------------------------------------------------------------------
glm::vec2 BoundingBox::GetOrigin() const
{
	return glm::vec2(m_Bounds.x, m_Bounds.y);
}

//---------------------------------------------------------------------------------------------------------------------
glm::vec2 BoundingBox::GetCenter() const
{
	const float width = GetWidth();
	const float height = GetHeight();

	return glm::vec2(m_Bounds.x + width / 2.0f, m_Bounds.y + height / 2.0f);
}

//---------------------------------------------------------------------------------------------------------------------
float BoundingBox::GetWidth() const
{
	return m_Bounds.z - m_Bounds.x;
}

//---------------------------------------------------------------------------------------------------------------------
float BoundingBox::GetHeight() const
{	
	return m_Bounds.w - m_Bounds.y;
}
