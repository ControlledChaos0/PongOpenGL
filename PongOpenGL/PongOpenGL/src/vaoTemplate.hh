template<typename T> void genBufferObject(GLuint& bo, GLenum type, GLuint noElements, T* data, GLenum usage);
template<typename T> void updateData(GLuint& bo, GLintptr offset, GLuint noElements, T* data);
template<typename T> void setAttPointer(GLuint& bo, GLuint idx, GLint size, GLenum type, GLuint stride, GLuint offset, GLuint divisor = 0);

//generate buffer of certain type and set data
template<typename T>
void genBufferObject(GLuint& bo, GLenum type, GLuint noElements, T* data, GLenum usage) {
	glGenBuffers(1, &bo);
	glBindBuffer(type, bo);
	glBufferData(type, noElements * sizeof(T), data, usage);
}

//update data in a buffer object
template<typename T>
void updateData(GLuint& bo, GLintptr offset, GLuint noElements, T* data) {
	glBindBuffer(GL_ARRAY_BUFFER, bo);
	glBufferSubData(GL_ARRAY_BUFFER, offset, noElements * sizeof(T), data);
}

//set attribute pointers
template<typename T>
void setAttPointer(GLuint& bo, GLuint idx, GLint size, GLenum type, GLuint stride, GLuint offset, GLuint divisor) {
	glBindBuffer(GL_ARRAY_BUFFER, bo);
	glVertexAttribPointer(idx, size, type, GL_FALSE, stride * sizeof(T), (void*)(offset * sizeof(T)));
	glEnableVertexAttribArray(idx);
	if (divisor > 0) {
		//reset idx attribute every divisor iteration through instances
		glVertexAttribDivisor(idx, divisor);
	}
}