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
	vao.Bind();
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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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

//GLfloat vertices[] =
//{
//	-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
//	0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
//	0.0f, 0.5f * float(sqrt(3)) * 2 / 3, 0.0f, // Upper corner
//	-0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner left
//	0.5f / 2, 0.5f * float(sqrt(3)) / 6, 0.0f, // Inner right
//	0.0f, -0.5f * float(sqrt(3)) / 3, 0.0f // Inner down
//};
//
//// Indices for vertices order
//GLuint indices[] =
//{
//	0, 3, 5, // Lower left triangle
//	3, 2, 4, // Lower right triangle
//	5, 4, 1 // Upper triangle
//};

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
	setOrthographicProjection(shader, 0, screenWidth, 0, screenHeight, 0.0f, 1.0f);

	//VAO VAO1;
	//VAO1.Bind();

	//VBO VBO1(vertices, sizeof(vertices), GL_STATIC_DRAW);
	//EBO EBO1(indices, sizeof(indices), GL_STATIC_DRAW);

	//VAO1.LinkVBO(VBO1, 0);
	//VAO1.Unbind();
	//VBO1.Unbind();
	//EBO1.Unbind();

	//while (!glfwWindowShouldClose(window))
	//{
	//	// Specify the color of the background
	//	glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
	//	// Clean the back buffer and assign the new color to it
	//	glClear(GL_COLOR_BUFFER_BIT);
	//	// Tell OpenGL which Shader Program we want to use
	//	shader.Activate();
	//	// Bind the VAO so OpenGL knows to use it
	//	VAO1.Bind();
	//	// Draw primitives, number of indices, datatype of indices, index of indices
	//	glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
	//	// Swap the back buffer with the front buffer
	//	glfwSwapBuffers(window);
	//	// Take care of all GLFW events
	//	glfwPollEvents();
	//}

	//VAO1.Delete();
	//VBO1.Delete();
	//EBO1.Delete();

	//setup vertex data
	float paddleVertices[] = {
		0.5f, 0.5f,
		-0.5f, 0.5f,
		-0.5f, -0.5f,
		0.5f, -0.5f
	};

	//setup index data
	unsigned int paddleIndices[] = {
		0, 1, 2,
		2, 3, 0
	};

	//offsets array
	float paddleOffsets[] = {
		35.0f, screenHeight / 2.0f,
		screenWidth - 35.0f, screenHeight / 2.0f
	};

	float paddleSizes[] = {
		paddleWidth, paddleHeight
	};

	paddleVelocities[0] = 0.0f;
	paddleVelocities[1] = 0.0f;

	VAO paddleVAO;
	paddleVAO.Bind();

	VBO paddlePosVBO(paddleVertices, 2 * 4, GL_STATIC_DRAW);
	paddleVAO.LinkVBO(paddlePosVBO, 0, 2);

	// offset VBO
	VBO paddleOffsetVBO(paddleOffsets, 2 * 2, GL_DYNAMIC_DRAW);
	paddleVAO.LinkVBO(paddleOffsetVBO, 1, 0, 1);

	//size VBO
	VBO paddleSizeVBO(paddleSizes, 2 * 1, GL_STATIC_DRAW);
	paddleVAO.LinkVBO(paddleSizeVBO, 2, 0, 2);

	//EBO
	EBO paddleEBO(paddleIndices, 2 * 4, GL_STATIC_DRAW);

	//unbind VBO and VAO
	paddleVAO.Unbind();
	paddlePosVBO.Unbind();
	paddleOffsetVBO.Unbind();
	paddleSizeVBO.Unbind();
	paddleEBO.Unbind();

	/*
		BALL VAO/BOS
	*/

	float* ballVertices;
	unsigned int* ballIndices;
	unsigned int numTriangles = 20;

	gen2DCircleArray(ballVertices, ballIndices, numTriangles, 0.5f);

	GLfloat ballOffset[] = {
		screenWidth / 2.0f, screenHeight / 2.0f
	};

	GLfloat ballSize[] = {
		ballDiameter, ballDiameter
	};

	//setup VAO/BOs
	VAO ballVAO;
	ballVAO.Bind();

	//pos VBO
	VBO ballPosVBO(ballVertices, 2 * (numTriangles + 1), GL_STATIC_DRAW);
	ballVAO.LinkVBO(ballPosVBO, 0);

	// offset VBO
	VBO ballOffsetVBO(ballOffset, 2, GL_DYNAMIC_DRAW);
	ballVAO.LinkVBO(ballOffsetVBO, 1, 0, 1);

	//size VBO
	VBO ballSizeVBO(ballSize, 2, GL_STATIC_DRAW);
	ballVAO.LinkVBO(ballSizeVBO, 2, 0, 1);

	//EBO
	EBO ballEBO = EBO(ballIndices, 3 * numTriangles, GL_STATIC_DRAW);

	//unbind VBO and VAO
	ballVAO.Unbind();
	ballPosVBO.Unbind();
	ballOffsetVBO.Unbind();
	ballSizeVBO.Unbind();
	ballEBO.Unbind();

	ballVelocity = initBallVelocity;

	unsigned int framesSinceLastCollision = -1;
	unsigned int framesThreshold = 10;

	//render loop
	while (!glfwWindowShouldClose(window)) {
		//update time
		dt = glfwGetTime() - lastFrame;
		lastFrame += dt;

		//input
		processPaddleInput(window, dt, paddleOffsets);

		/*
			physics
		*/

		if (framesSinceLastCollision != -1) {
			framesSinceLastCollision++;
		}

		paddleOffsets[1] += paddleVelocities[0] * dt;
		paddleOffsets[3] += paddleVelocities[1] * dt;

		//update position
		ballOffset[0] += ballVelocity.x * dt;
		ballOffset[1] += ballVelocity.y * dt;

		/*
			collision
		*/

		// playing field
		if (ballOffset[1] - ballRadius <= 0 || ballOffset[1] + ballRadius >= screenHeight) {
			ballVelocity.y *= -1;
		}

		unsigned char reset = 0;
		if (ballOffset[0] - ballRadius <= 0) {
			std::cout << "Right player point" << std::endl;
			reset = 1;
		}
		else if (ballOffset[0] + ballRadius >= screenWidth) {
			std::cout << "Left player point" << std::endl;
			reset = 2;
		}

		if (reset) {
			ballOffset[0] = screenWidth / 2.0f;
			ballOffset[1] = screenHeight / 2.0f;
			ballVelocity.x = reset == 1 ? initBallVelocity.x : -initBallVelocity.x;
			ballVelocity.y = initBallVelocity.y;
		}


		if (framesSinceLastCollision >= framesThreshold) {
			/*
			paddle collision
		*/
			int i = 0;
			if (ballOffset[0] > screenHeight / 2.0f) {
				//if ball on right side, check with right paddle
				i++;
			}

			//get distance from enter of ball to center of paddle
			vec2 distance = { abs(ballOffset[0] - paddleOffsets[(i * 2)]), abs(ballOffset[1] - paddleOffsets[(i * 2) + 1]) };

			//check if no collision possible
			if (distance.x <= halfPaddleWidth + ballRadius &&
				distance.y <= halfPaddleHeight + ballRadius) {
				bool collision = false;
				if (distance.x <= halfPaddleWidth && distance.x >= halfPaddleWidth - ballRadius) {
					collision = true;
					ballVelocity.x *= -1;
				}
				else if (distance.y <= halfPaddleHeight && distance.y >= halfPaddleHeight - ballRadius) {
					collision = true;
					ballVelocity.y *= -1;
				}

				float squaredistance = pow(distance.x - halfPaddleWidth, 2) + pow(distance.y - halfPaddleHeight, 2);
				if (squaredistance <= pow(ballRadius, 2)) {
					collision = true;
					ballVelocity.x *= -1;
				}

				if (collision) {
					float k = 0.5f;
					ballVelocity.x += .01f;
					ballVelocity.y += k * paddleVelocities[i];
					framesSinceLastCollision = 0;
				}
			}
		}

		/*
			graphics
		*/

		//clear screen for new frame
		clearScreen();

		//update
		/*updateData<float>(paddleOffsetVBO, 0, 2 * 2, paddleOffsets);
		updateData<float>(ballOffsetVBO, 0, 2, ballOffset);*/

		//render object
		shader.Activate();
		draw(paddleVAO, GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, 0, 2);
		draw(ballVAO, GL_TRIANGLES, 3 * numTriangles, GL_UNSIGNED_INT, 0);

		//swap frames
		newFrame(window);
	}

	//cleanup memory
	paddleVAO.Delete();
	paddlePosVBO.Delete();
	paddleOffsetVBO.Delete();
	paddleSizeVBO.Delete();
	paddleEBO.Delete();

	ballVAO.Delete();
	ballPosVBO.Delete();
	ballOffsetVBO.Delete();
	ballSizeVBO.Delete();
	ballEBO.Delete();

	shader.Delete();
	cleanup();

	return 0;
}
