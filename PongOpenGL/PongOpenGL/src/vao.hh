#ifndef VAO_HH
#define VAO_HH
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

#include "vaoTemplate.hh"

/*
	Vertex Array Object/Buffer Object Methods
*/
void genVAO(VAO* vao);
void draw(VAO vao, GLenum mode, GLuint count, GLenum type, GLint indices, GLuint instanceCount = 1);
void unbindBuffer(GLenum type);
void unbindVAO();
void cleanup(VAO vao);
#endif VAO_HH