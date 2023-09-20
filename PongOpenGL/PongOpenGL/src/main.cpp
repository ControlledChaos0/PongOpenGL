#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

unsigned int width = 800;
unsigned int height = 600;
const char* title = "Pong";

/*
	initialization methods
*/

// initialize methods
void initGLFW(unsigned int versionMajor, unsigned int versionMinor) {}

//create window
void createWindow(GLFWwindow*& window, const char* title, unsigned int width, unsigned int height, GLFWframebuffersizefun framebufferSizeCallback) {}

// callback window size change
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {}

//load glad library
bool loadGlad() {}

/*
	shader methods
*/

//read file
std::string readFile(const char* filename) {}

//generate shader
int genShader(const char* filepath, GLenum type) {}

//generate shader program
int genShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath) {}

//bind shader
void bindShader(int shaderProgram) {}

//set projection
void setOrthographicProjection(int shaderProgram, float left, float right, float bottom, float top, float near, float far) {}

//delete shader
void deleteShader(int shaderProgram) {}

/*
	Vertex Array Object/Buffer Object Methods
*/

//structure for VAO storing Array Object and its Buffer objects
struct VAO {
	GLuint val;
	GLuint posVBO;
	GLuint offsetVBO;
	GLuint sizeVBO;
	GLuint sizeVBO;
	GLuint EBO;
};

//generate VAO
void genVAO(VAO* vao) {}

//generate buffer of certain type and set data
template<typename T>
void genBufferObject(GLuint& bo, GLenum type, GLuint noElements, T* data, GLenum usage) {}

//update data in a buffer object
template<typename T>
void updateData(GLuint& bo, GLintptr offset, GLuint noElements, T* data) {}

//set attribute pointers
void setAttPointer(GLuint& bo, GLuint idx, GLint size, GLenum type, GLuint stride, GLuint divisor = 0) {}

//draw VAO
void draw(VAO vao, GLenum mode, GLuint count, GLenum type, GLint indices, GLuint instanceCount = 1) {}

//unbind buffer
void unbindBuffer(GLenum type) {}

//unbind VAO
void unbindVAO() {}

//deallocate VAO/VBO memory
void cleanup(VAO vao) {}

/*
	main loop methods
*/

//process input
void processInput(GLFWwindow* window) {}

//clear screen
void clearScreen() {}

// new frame
void newFrame(GLFWwindow* window) {}

/*
	clean up methods
*/

//terminate glfw
void cleanup() {}

int main() {
	std::cout << "Initializing Window" << std::endl;

	//timing
	double dt = 0.0;
	double lastFrame = 0.0;

	return 0;
}
