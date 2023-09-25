#include "VAO.hpp"

VAO::VAO() {
	glGenVertexArrays(1, &vaoObj);
}

void VAO::LinkAttri(VBO VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset, GLuint divisor) {
	VBO.Bind();
	glVertexAttribPointer(layout, numComponents, type, GL_FALSE, stride, offset);
	glEnableVertexAttribArray(layout);
	if (divisor > 0) {
		//reset idx attribute every divisor iteration through instances
		glVertexAttribDivisor(layout, divisor);
	}
	VBO.Unbind();
}

void VAO::Bind() {
	glBindVertexArray(vaoObj);
}

void VAO::Unbind() {
	glBindVertexArray(0);
}

void VAO::Delete() {
	glDeleteVertexArrays(1, &vaoObj);
}