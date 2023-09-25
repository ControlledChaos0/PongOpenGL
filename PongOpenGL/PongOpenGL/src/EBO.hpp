#ifndef EBO_H
#define EBO_H

#include <glad/glad.h>

class EBO {
	public:
		GLuint eboObj;
		EBO(GLuint* data, GLsizeiptr numElements, GLenum usage);

		void Bind();
		void Unbind();
		void Delete();
};

#endif