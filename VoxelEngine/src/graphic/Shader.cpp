#include "Shader.h"



Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath) {
	std::string vertCode;
	std::string fragCode;

	std::ifstream vertFile;
	std::ifstream fragFile;

	vertFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	vertFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vertFile.open(vertexPath);
		fragFile.open(fragmentPath);

		std::stringstream vertStream, fragStream;

		vertStream << vertFile.rdbuf();
		fragStream << fragFile.rdbuf();

		vertFile.close();
		fragFile.close();

		vertCode = vertStream.str();
		fragCode = fragStream.str();
	}
	catch (std::ifstream::failure e) {
		std::cout << "Error, while reading shader from file." << std::endl;
	}

	const char* vertexCode = vertCode.c_str();
	const char* fragmentCode = fragCode.c_str();

	unsigned int vertId, fragId;
	int error;

	vertId = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertId, 1, &vertexCode, nullptr);
	glCompileShader(vertId);
	glGetShaderiv(vertId, GL_COMPILE_STATUS, &error);
	if (!error) {
		char errorInfo[512];
		glGetShaderInfoLog(vertId, 512, nullptr, errorInfo);
		std::cout << "Error, while compiling vertex shader: " << errorInfo << std::endl;
	}

	fragId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragId, 1, &fragmentCode, nullptr);
	glCompileShader(fragId);
	glGetShaderiv(fragId, GL_COMPILE_STATUS, &error);
	if (!error) {
		char errorInfo[512];
		glGetShaderInfoLog(fragId, 512, nullptr, errorInfo);
		std::cout << "Error, while compiling fragment shader: " << errorInfo << std::endl;
	}

	programId_ = glCreateProgram();
	glAttachShader(programId_, vertId);
	glAttachShader(programId_, fragId);
	glLinkProgram(programId_);

	glGetProgramiv(programId_, GL_LINK_STATUS, &error);
	if(!error) {
		char errorInfo[512];
		glGetProgramInfoLog(programId_, 512, nullptr, errorInfo);
		std::cout << "Error, while linking shader program: " << errorInfo << std::endl;
	}

	glDeleteShader(vertId);
	glDeleteShader(fragId);
}


Shader::~Shader() {
}

void Shader::activate() {
	glUseProgram(programId_);
}

void Shader::setBool(const std::string & name, bool value) const {
	glUniform1i(glGetUniformLocation(programId_, name.c_str()), (int)value);
}

void Shader::setInt(const std::string & name, int value) const {
	glUniform1i(glGetUniformLocation(programId_, name.c_str()), value);
}

void Shader::setFloat(const std::string & name, float value) const {
	glUniform1f(glGetUniformLocation(programId_, name.c_str()), (int)value);
}

void Shader::setMat4(const std::string & name, glm::mat4& matrix) {
	glUniformMatrix4fv(glGetUniformLocation(programId_, name.c_str()), 1, GL_FALSE, glm::value_ptr(matrix) );

}
