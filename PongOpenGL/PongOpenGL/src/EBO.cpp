#include "EBO.hpp"

EBO::EBO(GLuint* data, GLsizeiptr numElements, GLenum usage) {
	glGenBuffers(1, &eboObj);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboObj);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, numElements * sizeof(GLuint), data, usage);
}

void EBO::Bind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eboObj);
}

void EBO::Unbind() {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void EBO::Delete() {
	glDeleteBuffers(1, &eboObj);
}