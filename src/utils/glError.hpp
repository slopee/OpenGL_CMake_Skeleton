/**
 * glError.hpp
 * Contributors:
 *      * Arthur Sonzogni (author)
 * Licence:
 *      * MIT
 */

#ifndef GLERROR_J4YD6FVF
#define GLERROR_J4YD6FVF

#include <GL/glew.h>

// Ask Opengl for errors:
// Result is printed on the standard output
// usage :
//      glCheckError(__FILE__,__LINE__);
void glCheckError(const char* file, unsigned int line);
void GLAPIENTRY glCallbackFunction(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const void* userParam);

#endif /* end of include guard: GLERROR_J4YD6FVF */

