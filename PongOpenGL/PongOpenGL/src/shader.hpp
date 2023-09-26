#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cerrno>

/*
	shader methods
*/
std::string readFile(const char* filename);
GLuint genShader(const char* filepath, GLenum type);
GLuint genShaderString(std::string filestring, GLenum type);

class Shader {
public:
	GLuint shaderObj;
	Shader(const char* vertexShaderFile, const char* fragmentShaderFile);
	Shader(std::string vertexShaderFile, std::string fragmentShaderFile);

	void Activate();
	void Delete();
};

void setOrthographicProjection(Shader shader,
	float left, float right,
	float bottom, float top,
	float near, float far);

#endif