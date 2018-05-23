/**
 * Application3D.cpp
 * Contributors:
 *      * Arthur Sonzogni (author)
 * Licence:
 *      * MIT
 */
#include "Application3D.hpp"
#include <GL/glew.h>
#include <vector>

#include <glm/glm.hpp>
#include <iostream>
#include "utils/glError.hpp"

#include <GLFW/glfw3.h>
#include <graphic/Camera.h>
#include <app/Quad.h>
#include <app/Cube.h>
#include <app/Grid.h>

Quad* m_Quad;
Cube* m_Cube;
Grid* m_Grid;

Application3D::Application3D():
    Application()
{
    glCheckError(__FILE__,__LINE__);	
	m_Quad = new Quad(1);
	m_Cube = new Cube(1);
	m_Grid = new Grid(glm::ivec2(4, 2), glm::vec3());
	m_Camera = new Camera();
}

void Application3D::loop()
{	
    // exit on window close button pressed
    if (glfwWindowShouldClose(getWindow()))
        exit();

    float t = getTime();
    // set matrix : projection + view
    
	glm::mat4 projection = m_Camera->GetProjectionMatrix(getHeight(), getWindowRatio());
	glm::mat4 view = m_Camera->GetViewMatrix();
	
    // clear
    glClear(GL_COLOR_BUFFER_BIT);
    glClearColor(0.0,0.0,0.0,0.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//m_Quad->Draw(t, projection, view);
	m_Grid->Draw(t, projection, view);
}
