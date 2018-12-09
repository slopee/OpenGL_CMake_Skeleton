#pragma once
#include "ShaderProgram.h"

class Camera;

class Renderable
{
public:
	Renderable(Shader vertexShader, Shader fragmentShader);
	virtual ~Renderable() = default;

	void BindShaderProgram();
	void UnbindShaderProgram();

	void Draw(float time, const Camera& camera);

protected:
	virtual void OnDraw(float time, const Camera& camera) = 0;

protected:
	Shader m_VertexShader;
	Shader m_FragmentShader;
	ShaderProgram m_ShaderProgram;
};
