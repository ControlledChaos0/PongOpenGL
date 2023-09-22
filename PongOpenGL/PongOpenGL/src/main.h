#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

unsigned int screenWidth = 800;
unsigned int screenHeight = 600;
const char* title = "Pong";
GLuint shaderProgram;

//graphics parameters
const float paddleSpeed = 130.0f;
const float paddleHeight = 100.0f;
const float paddleWidth = 15.0f;
const float ballDiameter = 10.0f;
const float paddleBoundary = (paddleHeight / 2.0f) + (ballDiameter / 2.0f);

const double pi = 3.14159265358979323846;
//structure for VAO storing Array Object and its Buffer objects
struct VAO {
	GLuint val;
	GLuint posVBO;
	GLuint offsetVBO;
	GLuint sizeVBO;
	GLuint EBO;
};

struct vec2 {
	float x;
	float y;
};

//public ball vel
vec2 ballVelocity;

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
void genBufferObject(GLuint& bo, GLenum type, GLuint numElements, T* data, GLenum usage);
template<typename T>
void updateData(GLuint& bo, GLintptr offset, GLuint numElements, T* data);
template<typename T>
void setAttPointer(GLuint& bo, GLuint idx, GLint size, GLenum type, GLuint stride, GLuint offset, GLuint divisor = 0);
void draw(VAO vao, GLenum mode, GLuint count, GLenum type, GLint indices, GLuint instanceCount = 1);
void unbindBuffer(GLenum type);
void unbindVAO();
void cleanup(VAO vao);

/*
	main loop methods
*/
void processPaddleInput(GLFWwindow* window, double dt, float* paddleOffset);
void clearScreen();
void newFrame(GLFWwindow* window);

/*
	clean up methods
*/
void cleanup();