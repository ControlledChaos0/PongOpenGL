#include "VBO.hpp"

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