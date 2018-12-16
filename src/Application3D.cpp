/**
 * Application3D.cpp
 * Contributors:
 *      * Arthur Sonzogni (author)
 * Licence:
 *      * MIT
 */
#include "Application3D.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "utils/glError.hpp"
#include <GLFW/glfw3.h>
#include <graphic/Camera.h>
#include <app/Grid.h>
#include "system/InputEvent.h"
#include "system/Input.h"
#include "debug/Axis.h"
#include "app/Quad.h"
#include "app/QuadTree.h"
#include "graphic/BoundingBox.h"
#include "app/Heightmap.h"

Grid* m_Grid;
Quad* m_Quad;
QuadTree* m_QuadTree;
Heightmap* m_Heightmap;

GLenum m_FillType = GL_LINE;

Application3D::Application3D():
	Application()
{
	glCheckError(__FILE__,__LINE__);	
	m_Grid = new Grid(glm::uvec2(16, 16));	
	m_Quad = new Quad(24, m_Grid);

	Transform topFace;
	topFace.position = glm::vec3(0, 0, 0);
	topFace.rotation = glm::vec3(90.0f, 0, 0);	
	m_QuadTree = new QuadTree(BoundingBox(-25, -25, 50, 50), glm::vec2(1, 80), topFace.GetTransformMatrix(), m_Grid);
	
	m_Camera = new Camera(getHeight(), getWindowRatio());	
	m_Heightmap = new Heightmap(12, 12);

	Input::GetInstance().RegisterInputEvent<KeyEvent>([&](const KeyEvent& key)
	{
		if(key.pressed)
		{
			if (m_FillType == GL_LINE)
			{
				m_FillType = GL_FILL;
			}
			else
			{
				m_FillType = GL_LINE;
			}
		}		
	}, GLFW_KEY_Q);
}

void Application3D::loop()
{
	// exit on window close button pressed
	if (glfwWindowShouldClose(getWindow()))
		exit();
	
	float t = getTime();
	// set matrix : projection + view

	const auto& projection = m_Camera->GetProjectionMatrix();
	const auto& view = m_Camera->GetViewMatrix();
		
	// clear
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, m_FillType);

	m_Heightmap->BindTexture();
	//Transform defaultT;
	//m_Grid->Draw(t, projection, view, defaultT);
	//m_Quad->Draw(t, *m_Camera);
	m_QuadTree->Draw(t, *m_Camera);

	// Debug camera info
	m_Camera->Draw(t, projection, view);	
}
