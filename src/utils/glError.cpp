/**
 * glError.cpp
 * Contributors:
 *      * Arthur Sonzogni (author)
 * Licence:
 *      * MIT
 */

#include "glError.hpp"
#include "termcolor.hpp"
#include <GL/glew.h>
#include <string>
#include <iostream>

using namespace std;

void glCheckError(const char* file, unsigned int line)
{
    GLenum errorCode = glGetError();

    while (errorCode != GL_NO_ERROR)
    {
        string fileString(file);
        string error = "unknown error";

        switch (errorCode)
        {
            case GL_INVALID_ENUM : error = "GL_INVALID_ENUM"; break;
            case GL_INVALID_VALUE : error = "GL_INVALID_VALUE"; break;
            case GL_INVALID_OPERATION : error = "GL_INVALID_OPERATION"; break;
            case GL_STACK_OVERFLOW : error = "GL_STACK_OVERFLOW"; break;
            case GL_STACK_UNDERFLOW : error = "GL_STACK_UNDERFLOW"; break;
            case GL_OUT_OF_MEMORY : error = "GL_OUT_OF_MEMORY"; break;
        }

        cerr<<"OpenglError : file="<<file<<" line="<<line<<" error:"<<error<<endl;
        errorCode = glGetError();
    }
}

void GLAPIENTRY glCallbackFunction(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam) {

	if(type == GL_DEBUG_TYPE_ERROR)
	{
		cout << termcolor::on_red;
	} else if(type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR || type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR)
	{
		cout << termcolor::on_yellow;
	}

	cout << endl << "---------------------opengl-callback-start------------" << endl;
	cout << "message: " << message << endl;
	cout << "type: ";
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		cout << "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		cout << "DEPRECATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		cout << "UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		cout << "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		cout << "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER:
		cout << "OTHER";
		break;
	}
	cout << endl;

	cout << "id: " << id << endl;
	cout << "severity: ";
	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:
		cout << "LOW";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		cout << "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		cout << "HIGH";
		break;
	}
	cout << endl;
	cout << "---------------------opengl-callback-end--------------" << termcolor::reset << endl << endl;
}
