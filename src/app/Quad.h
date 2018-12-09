#pragma once
#include <graphic/Shader.hpp>
#include <graphic/ShaderProgram.h>
#include "graphic/Renderable.h"

class Grid;

class Quad : public Renderable
{
public:
	Quad(float size, Grid* grid);
	void SetSize(float newSize);

protected:
	void OnDraw(float time, const Camera& camera) override;

private:
	float m_Size;
	Grid* m_Grid;
};
