/**
 * Shader.hpp
 * Contributors:
 *      * Arthur Sonzogni (author)
 * Licence:
 *      * MIT
 */


#ifndef SHADER_F8X43H2W
#define SHADER_F8X43H2W

#include <GL/glew.h>
#include <string>

#define GLM_FORCE_RADIANS
#define SHADER_DIR "../shader/"

class Shader
{
    public:
        // Load Shader from a file
        Shader(const std::string& filename, GLenum type);

        // provide opengl shader identifiant.
        GLuint getHandle() const;
        
        ~Shader();
    private:

        // opengl program identifiant
        GLuint handle;
};

#endif /* end of include guard: SHADER_F8X43H2W */
