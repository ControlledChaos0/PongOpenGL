#include "init.hh"
#include <string>
#include <sstream>
#include <fstream>
#include "vao.hh"

/*
	Vertex Array Object/Buffer Object Methods
*/

//generate VAO
void genVAO(VAO* vao) {
	glGenVertexArrays(1, &vao->val);
	glBindVertexArray(vao->val);
}

//draw VAO
void draw(VAO vao, GLenum mode, GLuint count, GLenum type, GLint indices, GLuint instanceCount) {
	glBindVertexArray(vao.val);
	glDrawElementsInstanced(mode, count, type, (void*)indices, instanceCount);
}

//unbind buffer
void unbindBuffer(GLenum type) {
	glBindBuffer(type, 0);
}

//unbind VAO
void unbindVAO() {
	glBindVertexArray(0);
}

//deallocate VAO/VBO memory
void cleanup(VAO vao) {
	glDeleteBuffers(1, &vao.posVBO);
	glDeleteBuffers(1, &vao.offsetVBO);
	glDeleteBuffers(1, &vao.sizeVBO);
	glDeleteBuffers(1, &vao.EBO);
	glDeleteVertexArrays(1, &vao.val);
}