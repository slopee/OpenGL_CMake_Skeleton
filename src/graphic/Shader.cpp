/**
 * Shader.cpp
 * Contributors:
 *      * Arthur Sonzogni (author)
 * Licence:
 *      * MIT
 */

#include "Shader.hpp"
#include <vector>
#include <fstream>
#include <iostream>
#include <cstdlib>
#include <stdexcept>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

// file reading
void getFileContents(const char *filename, vector<char>& buffer)
{
    //debug("chargement du fichier : %s",filename);
    ifstream file(filename, ios_base::binary);
    if (file)
    {
        file.seekg(0, ios_base::end);
        streamsize size = file.tellg();
        if (size > 0)
        {
            file.seekg(0, ios_base::beg);
            buffer.resize(static_cast<size_t>(size));
            file.read(&buffer[0], size);
        }
        buffer.push_back('\0');
    }
    else
    {
        throw std::invalid_argument(string("The file ") + filename + " doesn't exists");
    }
}


Shader::Shader(const std::string &filename, GLenum type)
{
    // file loading
    vector<char> fileContent;
    getFileContents(filename.c_str(),fileContent);

    // creation
    handle = glCreateShader(type);
    if(handle == 0)
        throw std::runtime_error("[Error] Impossible to create a new Shader");

    // code source assignation
    const char* shaderText(&fileContent[0]);
    glShaderSource(handle, 1, (const GLchar**)&shaderText, NULL);

    // compilation
    glCompileShader(handle);

    // compilation check
    GLint compile_status;
    glGetShaderiv(handle, GL_COMPILE_STATUS, &compile_status);
    if(compile_status != GL_TRUE)
    {
        /* error text retreiving*/
        GLsizei logsize = 0;
        glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &logsize);
         
        char* log = new char[logsize+1];
        glGetShaderInfoLog(handle, logsize, &logsize, log);
        //log[logsize]='\0';
         
        cout<<"[Error] compilation error: "<<filename<<endl;
        cout<<log<<endl;
        
        exit(EXIT_FAILURE);
    }
    else
    {
        cout<<"[Info] Shader "<<filename<<" compiled successfully"<<endl;
    }
}



GLuint Shader::getHandle() const
{
    return handle;
}

Shader::~Shader()
{
}
