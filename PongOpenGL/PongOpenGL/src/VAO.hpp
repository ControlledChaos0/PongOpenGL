#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>
#include "VBO.hpp"
#include "EBO.hpp"

class VAO {
public:
	GLuint vaoObj;

	VAO();
	void LinkAttri(VBO VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset, GLuint divisor = 0);
	void Bind();
	void Unbind();
	void Delete();
};

#endif
