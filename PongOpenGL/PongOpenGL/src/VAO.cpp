#include "VAO.hpp"

VAO::VAO() {
	glGenVertexArrays(1, &vaoObj);
}

//Vbo, which attribute in the shader is being linked (0, 1, 2, etc), number of components for each vertex (vec2, vec3, etc), what variable type, length of the chunks (MULTIPLY BY SIZEOF(VARIABLE)), where to start, how many of the objects will each be used on at a time (good for copying attributes to multiple instanced objects)
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