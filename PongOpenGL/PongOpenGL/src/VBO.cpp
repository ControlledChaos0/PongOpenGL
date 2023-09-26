#include "VBO.hpp"

//array with vertices, numElements (MULTIPLIED BY SIZEOF(VARIABLE), GL_STATIC_DRAW etc.)
VBO::VBO(GLfloat* vertices, GLsizeiptr numElements, GLenum usage) {
	glGenBuffers(1, &vboObj);
	glBindBuffer(GL_ARRAY_BUFFER, vboObj);
	glBufferData(GL_ARRAY_BUFFER, numElements, vertices, usage);
}


void VBO::Bind() {
	glBindBuffer(GL_ARRAY_BUFFER, vboObj);
}

void VBO::Unbind() {
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBO::Delete() {
	glDeleteBuffers(1, &vboObj);
}