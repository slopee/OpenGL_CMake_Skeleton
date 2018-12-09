#pragma once
#include <glm/detail/type_vec4.hpp>

class BoundingBox
{
public:
	BoundingBox(float x, float y, float width, float height);
	BoundingBox(const glm::vec2& center, float width, float height);
	~BoundingBox() = default;

	glm::vec2 GetOrigin() const;
	glm::vec2 GetCenter() const;
	float GetWidth() const;
	float GetHeight() const;	

private:
	glm::vec4 m_Bounds;
};
