#pragma once
#include <glad/glad.h>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

class Shader
{
public:
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	~Shader();
	void activate();
	void setBool(std::string& name, bool value) const;
	void setInt(std::string& name, int value) const;
	void setFloat(std::string& name, float value) const;
private:
	unsigned int programId_;
};

