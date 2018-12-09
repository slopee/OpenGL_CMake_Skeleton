#pragma once
#include <glm/mat4x3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <GL/glew.h>
#include "graphic/ShaderProgram.h"
#include <vector>
#include "graphic/Camera.h"

class Transform;

class CameraDebugDrawer
{
public:
	CameraDebugDrawer(
		const Camera::LookAtVectors& cameraLookAtVector,
		float fov,
		float nearDist,
		float farDist,
		float ratio);


	void Update(
		const Camera::LookAtVectors& cameraLookAtVector,
		float fov,
		float nearDist,
		float farDist,
		float ratio);
	void Draw(float time, const glm::mat4& projection, const glm::mat4& view);

private:
	std::vector<glm::vec3> CalculateVerticesPositions(
		const Camera::LookAtVectors& cameraLookAtVector,
		float fov,
		float nearDist,
		float farDist,
		float ratio);
	
private:	
	GLuint m_Vao;
	GLuint m_Vbo;

	ShaderProgram m_ShaderProgram;
};
