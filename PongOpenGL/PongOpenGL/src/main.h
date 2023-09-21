#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

//structure for VAO storing Array Object and its Buffer objects
struct VAO {
	GLuint val;
	GLuint posVBO;
	GLuint offsetVBO;
	GLuint sizeVBO;
	GLuint EBO;
};

/*
	initialization methods
*/
void initGLFW(unsigned int versionMajor, unsigned int versionMinor);
void createWindow(GLFWwindow*& window, const char* title, unsigned int width, unsigned int height, GLFWframebuffersizefun framebufferSizeCallback);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
bool loadGlad();

/*
	shader methods
*/
std::string readFile(const char* filename);
int genShader(const char* filepath, GLenum type);
int genShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath);
void bindShader(int shaderProgram);
void setOrthographicProjection(int shaderProgram,
	float left, float right,
	float bottom, float top,
	float near, float far);
void deleteShader(int shaderProgram);

/*
	Vertex Array Object/Buffer Object Methods
*/
void genVAO(VAO* vao);
template<typename T>
void genBufferObject(GLuint& bo, GLenum type, GLuint noElements, T* data, GLenum usage);
template<typename T>
void updateData(GLuint& bo, GLintptr offset, GLuint noElements, T* data);
template<typename T>
void setAttPointer(GLuint& bo, GLuint idx, GLint size, GLenum type, GLuint stride, GLuint offset, GLuint divisor = 0);
void draw(VAO vao, GLenum mode, GLuint count, GLenum type, GLint indices, GLuint instanceCount = 1);
void unbindBuffer(GLenum type);
void unbindVAO();
void cleanup(VAO vao);

/*
	main loop methods
*/
void processInput(GLFWwindow* window);
void clearScreen();
void newFrame(GLFWwindow* window);

/*
	clean up methods
*/
void cleanup();