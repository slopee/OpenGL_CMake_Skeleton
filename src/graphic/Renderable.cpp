#include "Renderable.h"
#include <graphic/Camera.h>

//---------------------------------------------------------------------------------------------------------------------
Renderable::Renderable(Shader vertexShader, Shader fragmentShader) :
	m_VertexShader(vertexShader),
	m_FragmentShader(fragmentShader),
	m_ShaderProgram({ m_VertexShader, m_FragmentShader })
{
}

//---------------------------------------------------------------------------------------------------------------------
void Renderable::BindShaderProgram()
{
	m_ShaderProgram.use();
}

//---------------------------------------------------------------------------------------------------------------------
void Renderable::UnbindShaderProgram()
{
	m_ShaderProgram.unuse();
}

//---------------------------------------------------------------------------------------------------------------------
void Renderable::Draw(float time, const Camera& camera)
{
	BindShaderProgram();

	// Set the uniforms
	m_ShaderProgram.setUniform("projection", camera.GetProjectionMatrix());
	m_ShaderProgram.setUniform("view", camera.GetViewMatrix());

	OnDraw(time, camera);

	UnbindShaderProgram();
}
