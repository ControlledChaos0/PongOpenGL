#ifndef INIT_HH
#define INIT_HH
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

extern unsigned int width;
extern unsigned int height;
extern const char* title;
extern GLuint shaderProgram;

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
void bindShader(GLuint shaderProgram);
void setOrthographicProjection(GLuint shaderProgram,
	float left, float right,
	float bottom, float top,
	float near, float far);
void deleteShader(GLuint shaderProgram);

/*
	main loop methods
*/
void processInput(GLFWwindow* window, float* offset);
void clearScreen();
void newFrame(GLFWwindow* window);

/*
	clean up methods
*/
void cleanup();
#endif INIT_HH