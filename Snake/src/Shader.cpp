#include "Shader.h"

Shader::Shader() : mHandle(0){}

bool Shader::LoadShaders(const char* vsFilename, const char* fsFilename){
	string vertCodeCpy = fileToString(vsFilename);
	string fragCodeCpy = fileToString(fsFilename);

	const GLchar* vertCode = vertCodeCpy.c_str();
	const GLchar* fragCode = fragCodeCpy.c_str();

	GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertShader, 1, &vertCode, NULL);
	glCompileShader(vertShader); 
	
	checkCompileErrors(vertShader, VERTEX);

	glShaderSource(fragShader, 1, &fragCode, NULL);
	glCompileShader(fragShader);

	checkCompileErrors(vertShader, FRAGMENT);

	mHandle = glCreateProgram();
	glAttachShader(mHandle, vertShader);
	glAttachShader(mHandle, fragShader);
	glLinkProgram(mHandle);

	checkCompileErrors(0, PROGRAM);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
	
	mUniformLocations.clear();

	return true;
}

void Shader::use(){
	if (mHandle > 0)
		glUseProgram(mHandle);
}

void Shader::setUniform(const GLchar* name, const glm::vec2& v){
	GLint loc = getUniformLocation(name);
	glUniform2f(loc, v.x, v.y);
}

void Shader::setUniform(const GLchar* name, const glm::vec3& v){
	GLint loc = getUniformLocation(name);
	glUniform3f(loc, v.x, v.y, v.z);
}

void Shader::setUniform(const GLchar* name, const glm::vec4& v){
	GLint loc = getUniformLocation(name);
	glUniform4f(loc, v.x, v.y, v.z, v.w);
}

void Shader::setUniform(const GLchar* name, const glm::mat4& m){
	GLint loc = getUniformLocation(name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
}

GLuint Shader::getProgram()
{
	return mHandle;
}

Shader::~Shader(){
	glDeleteProgram(mHandle);
}

string Shader::fileToString(const string& filename){
	std::stringstream ss;
	std::ifstream file;

	try {
		file.open(filename, std::ios::in);

		if (!file.fail()) {
			ss << file.rdbuf();
		}

		file.close();
	}
	catch (std::exception ex) {
		std::cerr << "Error reading shader filename!..." << std::endl;
	}
	return ss.str();
}

void Shader::checkCompileErrors(GLuint shaderID, ShaderType type){
	int status = 0;

	if (type == PROGRAM) {
		glGetProgramiv(mHandle, GL_LINK_STATUS, &status);
		if (status == GL_FALSE) {
			GLint lenght = 0;
			glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &lenght);

			string errorLog(lenght, ' ');
			glGetProgramInfoLog(mHandle, lenght, &lenght, &errorLog[0]);
			std::cerr << "Error program failed to link heres the error: " << errorLog << std::endl;
		}
	} 
	else { //VERTEX or FRAGMENT
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &status);
		if (status == GL_FALSE) {
			GLint lenght = 0;
			glGetShaderiv(mHandle, GL_INFO_LOG_LENGTH, &lenght);

			string errorLog(lenght, ' ');
			glGetShaderInfoLog(mHandle, lenght, &lenght, &errorLog[0]);
			std::cerr << "Error shader failed to compile heres the error: " << errorLog << std::endl;
		}
	}
}

GLint Shader::getUniformLocation(const GLchar* name){
	std::map<string, GLint>::iterator it = mUniformLocations.find(name);

	if (it == mUniformLocations.end()) {
		mUniformLocations[name] = glGetUniformLocation(mHandle, name);
	}

	return mUniformLocations[name];
}
