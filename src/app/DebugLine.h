#pragma once
#include "graphic/ShaderProgram.h"

class Camera;

class DebugLine
{
public:
	static void DrawLine(const glm::vec3& origin, const glm::vec3& dest, const Camera& camera);

private:
	static void InitOpenGLContext();
};
