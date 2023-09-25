#ifndef VBO_H
#define VBO_H

#include <glad/glad.h>

class VBO {
	public:
		GLuint vboObj;
		VBO(GLfloat* vertices, GLsizeiptr numElements, GLenum usage);

		void Bind();
		void Unbind();
		void Delete();
};

#endif
