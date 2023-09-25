#include <string>
#include <sstream>
#include <fstream>
#include "main.hpp"
#include "shader.hpp"
#include "VAO.hpp"
#include "VBO.hpp"
#include "EBO.hpp"

/*
	initialization methods
*/

// initialize methods
void initGLFW(unsigned int versionMajor, unsigned int versionMinor) {
	glfwInit();

	//pass in window params
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, versionMajor);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, versionMinor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
}

//create window
void createWindow(GLFWwindow*& window, const char* title, unsigned int width, unsigned int height, GLFWframebuffersizefun framebufferSizeCallback) {
	window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!window) {
		cleanup();
		return;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
}

// callback window size change
void framebufferSizeCallback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, screenWidth, screenHeight);
}

//load glad library
bool loadGlad() {
	return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

/*
	shader methods
*/

//set projection
void setOrthographicProjection(Shader shader,
	float left, float right,
	float bottom, float top,
	float near, float far) {
	/*float mat[4][4] = {
		{2.0f / (right - left), 0.0f, 0.0f, -(right + left) / (right - left)},
		{0.0f, 2.0f / (top - bottom), 0.0f, -(top + bottom) / (top - bottom)},
		{0.0f, 0.0f, -2.0f / (far - near), -(far + near) / (far - near)},
		{0.0f, 0.0f, 0.0f, 1.0f}
	};*/
	float mat[4][4] = {
		{ 2.0f / (right - left), 0.0f, 0.0f, 0.0f },
		{ 0.0f, 2.0f / (top - bottom), 0.0f, 0.0f },
		{ 0.0f, 0.0f, -2.0f / (far - near), 0.0f },
		{ -(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1.0f }
	};

	shader.Activate();
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderObj, "projection"), 1, GL_FALSE, &mat[0][0]);
}

/*
	Vertex Array Object/Buffer Object Methods
*/

template<typename T>
void updateData(VBO& bo, GLintptr offset, GLuint numElements, T* data) {
	bo.Bind();
	glBufferSubData(GL_ARRAY_BUFFER, offset, numElements * sizeof(T), data);
}

//draw VAO
void draw(VAO vao, GLenum mode, GLuint count, GLenum type, GLint indices, GLuint instanceCount) {
	//vao.Bind();
	glDrawElementsInstanced(mode, count, type, (void*)indices, instanceCount);
}

//method to generate arrays for circle model
void gen2DCircleArray(float*& vertices, unsigned int*& indices, unsigned int numTriangles, float radius = 0.5f) {
	vertices = new float[(numTriangles + 1) * 2];
	vertices[0] = 0.0f;
	vertices[1] = 0.0f;

	indices = new unsigned int[numTriangles * 3];

	float numTrianglesF = (float)numTriangles;
	float theta = 0.0f;

	for (unsigned int i = 0; i < numTriangles; i++) {
		vertices[(i + 1) * 2] = radius * cosf(theta);
		vertices[(i + 1) * 2 + 1] = radius * sinf(theta);

		indices[i * 3 + 0] = 0;
		indices[i * 3 + 1] = i + 1;
		indices[i * 3 + 2] = i + 2;

		theta += (2 * pi) / numTriangles;
	}

	indices[(numTriangles - 1) * 3 + 2] = 1;
}

/*
	main loop methods
*/

//process input
void processPaddleInput(GLFWwindow* window, double dt, float* paddleOffset) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	paddleVelocities[0] = 0.0f;
	paddleVelocities[1] = 0.0f;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		if (paddleOffset[3] < screenHeight - paddleBoundary) {
			paddleVelocities[1] = paddleSpeed;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		if (paddleOffset[3] > paddleBoundary) {
			paddleVelocities[1] = -paddleSpeed;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		if (paddleOffset[1] < screenHeight - paddleBoundary) {
			paddleVelocities[0] = paddleSpeed;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		if (paddleOffset[1] > paddleBoundary) {
			paddleVelocities[0] = -paddleSpeed;
		}
	}
}

//clear screen
void clearScreen() {
	glClearColor(0.0f, 0.2f, 0.2f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

// new frame
void newFrame(GLFWwindow* window) {
	glfwSwapBuffers(window);
	glfwPollEvents();
}

/*
	clean up methods
*/

//terminate glfw
void cleanup() {
	glfwTerminate();
}

GLfloat vertices[] =
{
	-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
	0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
	0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
	-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
	0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner right
	0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Inner down
};

// Indices for vertices order
GLuint indices[] =
{
	0, 3, 5, // Lower left triangle
	3, 2, 4, // Lower right triangle
	5, 4, 1 // Upper triangle
};

int main() {
	std::cout << "Initializing Window" << std::endl;

	//timing
	double dt = 0.0;
	double lastFrame = 0.0;

	//initialization
	initGLFW(3, 3);

	//create window
	GLFWwindow* window = nullptr;
	createWindow(window, title, screenWidth, screenHeight, framebufferSizeCallback);
	if (!window) {
		std::cout << "Could not create window" << std::endl;
		cleanup();
		return -1;
	}

	//load glad
	if (!loadGlad()) {
		std::cout << "Could not init GLAD" << std::endl;
		cleanup();
		return -1;
	}

	glViewport(0, 0, screenWidth, screenHeight);

	//shaders
	Shader shader("assets/vertexShader.glsl", "assets/fragmentShader.glsl");
	shader.Activate();
	//setOrthographicProjection(shader, 0, screenWidth, 0, screenHeight, 0.0f, 1.0f);

	//setup vertex data
	GLfloat paddleVertices[] = {
		0.5f, 0.5f,
		-0.5f, 0.5f,
		-0.5f, -0.5f,
		0.5f, -0.5f
	};

	//setup index data
	GLuint paddleIndices[] = {
		0, 1, 2,
		2, 3, 0
	};

	//offsets array
	GLfloat paddleOffsets[] = {
		35.0f, screenHeight / 2.0f,
		screenWidth - 35.0f, screenHeight / 2.0f
	};

	GLfloat paddleSizes[] = {
		paddleWidth, paddleHeight
	};

	VAO paddleVAO;
	paddleVAO.Bind();

	VBO paddlePosVBO(paddleVertices, sizeof(paddleVertices), GL_STATIC_DRAW);
	EBO paddleIndEBO(paddleIndices, sizeof(paddleIndices), GL_STATIC_DRAW);

	paddleVAO.LinkVBO(paddlePosVBO, 0);
	paddleVAO.Unbind();
	paddlePosVBO.Unbind();
	paddleIndEBO.Unbind();

	while (!glfwWindowShouldClose(window)) {
		clearScreen();

		shader.Activate();
		paddleVAO.Bind();
		//draw(paddleVAO, GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, 0, 2);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	paddleVAO.Delete();
	paddlePosVBO.Delete();
	paddleIndEBO.Delete();
	shader.Delete();
	cleanup();

	return 0;
}
