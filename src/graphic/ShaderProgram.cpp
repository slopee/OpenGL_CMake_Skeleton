#include "ShaderProgram.h"

#include <iostream>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

ShaderProgram::ShaderProgram()
{
	// programme creation
	handle = glCreateProgram();
	if (!handle)
		throw std::runtime_error("Impossible to create a new shader program");
}


ShaderProgram::ShaderProgram(std::initializer_list<Shader> shaderList) :
	ShaderProgram()
{
	for (auto& s : shaderList)
		glAttachShader(handle, s.getHandle());

	link();
}


void ShaderProgram::link()
{
	glLinkProgram(handle);
	GLint result;
	glGetProgramiv(handle, GL_LINK_STATUS, &result);
	if (result != GL_TRUE)
	{
		cout << "[Error] linkage error" << endl;

		/* error text retreiving*/
		GLsizei logsize = 0;
		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &logsize);

		char* log = new char[logsize];
		glGetProgramInfoLog(handle, logsize, &logsize, log);
		//log[logsize]='\0';

		cout << log << endl;
	}
}

GLint ShaderProgram::uniform(const std::string &name)
{
	auto it = uniforms.find(name);
	if (it == uniforms.end())
	{
		// uniform that is not referenced
		GLint r = glGetUniformLocation(handle, name.c_str());
		if (r == GL_INVALID_OPERATION || r < 0)
			cout << "[Error] uniform " << name << " doesn't exist in program" << endl;
		// add it anyways
		uniforms[name] = r;

		return r;
	}
	else
		return it->second;
}

GLint ShaderProgram::attribute(const std::string& name)
{
	GLint attrib = glGetAttribLocation(handle, name.c_str());
	if (attrib == GL_INVALID_OPERATION || attrib < 0)
		cout << "[Error] Attribute " << name << " doesn't exist in program" << endl;

	return attrib;
}

void ShaderProgram::setAttribute(const std::string& name, GLint size, GLsizei stride, GLuint offset,
	GLboolean normalize,
	GLenum type)
{
	GLint loc = attribute(name);
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(
		loc,
		size,
		type,
		normalize,
		stride,
		reinterpret_cast<void*>(offset)
	);
}

void ShaderProgram::setAttribute(const std::string& name, GLint size, GLsizei stride, GLuint offset,
	GLboolean normalize)
{
	setAttribute(name, size, stride, offset, normalize, GL_FLOAT);
}

void ShaderProgram::setAttribute(const std::string& name, GLint size, GLsizei stride, GLuint offset,
	GLenum type)
{
	setAttribute(name, size, stride, offset, false, type);
}

void ShaderProgram::setAttribute(const std::string& name, GLint size, GLsizei stride, GLuint offset)
{
	setAttribute(name, size, stride, offset, false, GL_FLOAT);
}


void ShaderProgram::setUniform(const std::string& name, float x, float y, float z)
{
	glUniform3f(uniform(name), x, y, z);
}
void ShaderProgram::setUniform(const std::string& name, const vec3 & v)
{
	glUniform3fv(uniform(name), 1, value_ptr(v));
}
void ShaderProgram::setUniform(const std::string& name, const dvec3 & v)
{
	glUniform3dv(uniform(name), 1, value_ptr(v));
}
void ShaderProgram::setUniform(const std::string& name, const vec4 & v)
{
	glUniform4fv(uniform(name), 1, value_ptr(v));
}
void ShaderProgram::setUniform(const std::string& name, const dvec4 & v)
{
	glUniform4dv(uniform(name), 1, value_ptr(v));
}
void ShaderProgram::setUniform(const std::string& name, const dmat4 & m)
{
	glUniformMatrix4dv(uniform(name), 1, GL_FALSE, value_ptr(m));
}
void ShaderProgram::setUniform(const std::string& name, const mat4 & m)
{
	glUniformMatrix4fv(uniform(name), 1, GL_FALSE, value_ptr(m));
}
void ShaderProgram::setUniform(const std::string& name, const mat3 & m)
{
	glUniformMatrix3fv(uniform(name), 1, GL_FALSE, value_ptr(m));
}
void ShaderProgram::setUniform(const std::string& name, float val)
{
	glUniform1f(uniform(name), val);
}
void ShaderProgram::setUniform(const std::string& name, int val)
{
	glUniform1i(uniform(name), val);
}

ShaderProgram::~ShaderProgram()
{
	//glDeleteProgram(handle);
}


void ShaderProgram::use() const
{
	glUseProgram(handle);
}
void ShaderProgram::unuse() const
{
	glUseProgram(0);
}

GLuint ShaderProgram::getHandle() const
{
	return handle;
}
