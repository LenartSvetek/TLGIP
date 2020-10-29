#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
using std::string;

class Shader {
public:
	Shader();

	enum ShaderType {
		VERTEX,
		FRAGMENT,
		PROGRAM
	};

	bool LoadShaders(const char* vsFilename, const char* fsFilename);
	void use();

	void setUniform(const GLchar* name, const glm::vec2& v);
	void setUniform(const GLchar* name, const glm::vec3& v);
	void setUniform(const GLchar* name, const glm::vec4& v);
	void setUniform(const GLchar* name, const glm::mat4& m);

	GLuint getProgram();

	~Shader();

private:
	string fileToString(const string& filename);

	void checkCompileErrors(GLuint shaderID, ShaderType type);
	
	GLint getUniformLocation(const GLchar* name);

	GLuint mHandle;
	std::map<string, GLint> mUniformLocations;
};

#endif