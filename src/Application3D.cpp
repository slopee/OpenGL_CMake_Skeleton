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
#include <app/Quad.h>
#include <app/Cube.h>
#include <app/Grid.h>
#include "system/InputEvent.h"
#include "system/Input.h"
#include "debug/Axis.h"

Quad* m_Quad;
Cube* m_Cube;
Grid* m_Grid;

GLenum m_FillType = GL_LINE;

Application3D::Application3D():
	Application()
{
	glCheckError(__FILE__,__LINE__);
	m_Quad = new Quad(1);
	m_Grid = new Grid(glm::ivec2(10, 10));
	m_Cube = new Cube(2, m_Grid);
	m_Camera = new Camera(getHeight(), getWindowRatio());	

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
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPolygonMode(GL_FRONT_AND_BACK, m_FillType);

	//m_Quad->Draw(t, projection, view);	
	//m_Grid->Draw(t, projection, view, transform);	
	m_Cube->Draw(t, projection, view);
	m_Camera->Draw(t, projection, view);	
}
