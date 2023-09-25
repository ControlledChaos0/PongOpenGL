#include "VAO.hpp"

VAO::VAO() {
	glGenVertexArrays(1, &vaoObj);
}

void VAO::LinkVBO(VBO VBO, GLuint layout, GLuint offset, GLuint divisor) {
	VBO.Bind();
	glVertexAttribPointer(layout, 3, GL_FLOAT, GL_FALSE, 0, (void*)(offset));
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