#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "VAO.hpp"
#include <iostream>

unsigned int screenWidth = 800;
unsigned int screenHeight = 600;
const char* title = "Pong";

//graphics parameters
const float paddleSpeed = 250.0f;
const float paddleHeight = 100.0f;
const float halfPaddleHeight = paddleHeight / 2.0f;
const float paddleWidth = 10.0f;
const float halfPaddleWidth = paddleWidth / 2.0f;
const float ballDiameter = 10.0f;
const float ballRadius = ballDiameter / 2.0f;
const float paddleBoundary = (paddleHeight / 2.0f) + (ballDiameter / 2.0f);

const double pi = 3.14159265358979323846;
//structure for VAO storing Array Object and its Buffer objects
//struct VAO {
//	GLuint val;
//	GLuint posVBO;
//	GLuint offsetVBO;
//	GLuint sizeVBO;
//	GLuint EBO;
//};

struct vec2 {
	float x;
	float y;
};

//public vel
float paddleVelocities[2];
vec2 initBallVelocity = { 150.0f, 150.0f };
vec2 ballVelocity;

/*
	initialization methods
*/
void initGLFW(unsigned int versionMajor, unsigned int versionMinor);
void createWindow(GLFWwindow*& window, const char* title, unsigned int width, unsigned int height, GLFWframebuffersizefun framebufferSizeCallback);
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
bool loadGlad();

/*
	Vertex Array Object/Buffer Object Methods
*/
template<typename T>
void updateData(VBO& bo, GLintptr offset, GLuint numElements, T* data);
void draw(VAO vao, GLenum mode, GLuint count, GLenum type, GLint indices, GLuint instanceCount = 1);

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