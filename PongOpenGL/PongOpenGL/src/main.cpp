#include <string>
#include <sstream>
#include <fstream>
#include "main.h"

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

//read file
std::string readFile(const char* filename) {
	std::ifstream file;
	std::stringstream buf;

	std::string ret = "";

	//open file
	file.open(filename);

	if (file.is_open()) {
		buf << file.rdbuf();
		ret = buf.str();
	}
	else {
		std::cout << "Could not open " << filename << std::endl;
	}

	file.close();

	return ret;
}

//generate shader
int genShader(const char* filepath, GLenum type) {
	std::string shaderSrc = readFile(filepath);
	const GLchar* shader = shaderSrc.c_str();

	//build and compile the shader
	int shaderObj = glCreateShader(type);
	glShaderSource(shaderObj, 1, &shader, NULL);
	glCompileShader(shaderObj);

	//check for errors
	int success;
	char infoLog[512];
	glGetShaderiv(shaderObj, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderObj, 512, NULL, infoLog);
		std::cout << "Error in shader compilation:" << std::endl << infoLog << std::endl;
		return -1;
	}

	return shaderObj;
}

//generate shader program
int genShaderProgram(const char* vertexShaderPath, const char* fragmentShaderPath) {
	int shaderProgram = glCreateProgram();

	//compile shaders
	int vertexShader = genShader(vertexShaderPath, GL_VERTEX_SHADER);
	int fragmentShader = genShader(fragmentShaderPath, GL_FRAGMENT_SHADER);

	if (vertexShader == -1 || fragmentShader == -1) {
		return -1;
	}

	//link shader
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	//check for errors
	int success;
	char infoLog[512];
	glGetShaderiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "Error in shader linking:" << std::endl << infoLog << std::endl;
		return -1;
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	return shaderProgram;
}

//bind shader
void bindShader(int shaderProgram) {
	glUseProgram(shaderProgram);
}

//set projection
void setOrthographicProjection(int shaderProgram,
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

	bindShader(shaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &mat[0][0]);
}

//delete shader
void deleteShader(int shaderProgram) {
	glDeleteProgram(shaderProgram);
}

/*
	Vertex Array Object/Buffer Object Methods
*/

//generate VAO
void genVAO(VAO* vao) {
	glGenVertexArrays(1, &vao->val);
	glBindVertexArray(vao->val);
}

//generate buffer of certain type and set data
template<typename T>
void genBufferObject(GLuint& bo, GLenum type, GLuint numElements, T* data, GLenum usage) {
	glGenBuffers(1, &bo);
	glBindBuffer(type, bo);
	glBufferData(type, numElements * sizeof(T), data, usage);
}

//update data in a buffer object
template<typename T>
void updateData(GLuint& bo, GLintptr offset, GLuint numElements, T* data) {
	glBindBuffer(GL_ARRAY_BUFFER, bo);
	glBufferSubData(GL_ARRAY_BUFFER, offset, numElements * sizeof(T), data);
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
	glDeleteBuffers(1, & vao.EBO);
	glDeleteVertexArrays(1, &vao.val);
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
void processPaddleInput(GLFWwindow* window, double dt, vec2* paddleOffset) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	paddleVelocities[0] = 0.0f;
	paddleVelocities[1] = 0.0f;

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		if (paddleOffset[1].y < screenHeight - paddleBoundary) {
			paddleVelocities[1] = paddleSpeed;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		if (paddleOffset[1].y > paddleBoundary) {
			paddleVelocities[1] = -paddleSpeed;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		if (paddleOffset[0].y < screenHeight - paddleBoundary) {
			paddleVelocities[0] = paddleSpeed;
		}
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		if (paddleOffset[0].y > paddleBoundary) {
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
	shaderProgram = genShaderProgram("assets/vertexShader.glsl", "assets/fragmentShader.glsl");
	setOrthographicProjection(shaderProgram, 0, screenWidth, 0, screenHeight, 0.0f, 1.0f);

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
	vec2 paddleOffsets[] = {
		35.0f, screenHeight / 2.0f,
		screenWidth - 35.0f, screenHeight / 2.0f
	};

	vec2 paddleSizes[] = {
		paddleWidth, paddleHeight
	};

	paddleVelocities[0] = 0.0f;
	paddleVelocities[1] = 0.0f;

	VAO paddleVAO;
	genVAO(&paddleVAO);

	genBufferObject<float>(paddleVAO.posVBO, GL_ARRAY_BUFFER, 2 * 4, paddleVertices, GL_STATIC_DRAW);
	setAttPointer<float>(paddleVAO.posVBO, 0, 2, GL_FLOAT, 2, 0);

	// offset VBO
	genBufferObject<vec2>(paddleVAO.offsetVBO, GL_ARRAY_BUFFER, 2, paddleOffsets, GL_DYNAMIC_DRAW);
	setAttPointer<float>(paddleVAO.offsetVBO, 1, 2, GL_FLOAT, 2, 0, 1);

	//size VBO
	genBufferObject<vec2>(paddleVAO.sizeVBO, GL_ARRAY_BUFFER, 2, paddleSizes, GL_STATIC_DRAW);
	setAttPointer<float>(paddleVAO.sizeVBO, 2, 2, GL_FLOAT, 2, 0, 2);

	//EBO
	genBufferObject<GLuint>(paddleVAO.EBO, GL_ELEMENT_ARRAY_BUFFER, 2 * 4, paddleIndices, GL_STATIC_DRAW);

	//unbind VBO and VAO
	unbindBuffer(GL_ARRAY_BUFFER);
	unbindVAO();

	/*
		BALL VAO/BOS
	*/

	float* ballVertices;
	unsigned int* ballIndices;
	unsigned int numTriangles = 20;

	gen2DCircleArray(ballVertices, ballIndices, numTriangles, 0.5f);

	vec2 ballOffset = {
		screenWidth / 2.0f, screenHeight / 2.0f
	};

	vec2 ballSize = {
		ballDiameter, ballDiameter
	};

	//setup VAO/BOs
	VAO ballVAO;
	genVAO(&ballVAO);

	//pos VBO
	genBufferObject<float>(ballVAO.posVBO, GL_ARRAY_BUFFER, 2 * (numTriangles + 1), ballVertices, GL_STATIC_DRAW);
	setAttPointer<float>(ballVAO.posVBO, 0, 2, GL_FLOAT, 2, 0);

	// offset VBO
	genBufferObject<vec2>(ballVAO.offsetVBO, GL_ARRAY_BUFFER, 1, &ballOffset, GL_DYNAMIC_DRAW);
	setAttPointer<float>(ballVAO.offsetVBO, 1, 2, GL_FLOAT, 2, 0, 1);

	//size VBO
	genBufferObject<vec2>(ballVAO.sizeVBO, GL_ARRAY_BUFFER, 1, &ballSize, GL_STATIC_DRAW);
	setAttPointer<float>(ballVAO.sizeVBO, 2, 2, GL_FLOAT, 2, 0, 1);

	//EBO
	genBufferObject<unsigned int>(ballVAO.EBO, GL_ELEMENT_ARRAY_BUFFER, 3 * numTriangles, ballIndices, GL_STATIC_DRAW);

	//unbind VBO and VAO
	unbindBuffer(GL_ARRAY_BUFFER);
	unbindVAO(); 

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

		paddleOffsets[0].y += paddleVelocities[0] * dt;
		paddleOffsets[1].y += paddleVelocities[1] * dt;

		//update position
		ballOffset.x += ballVelocity.x * dt;
		ballOffset.y += ballVelocity.y * dt;

		/*
			collision
		*/

		// playing field
		if (ballOffset.y - ballRadius <= 0 || ballOffset.y + ballRadius >= screenHeight) {
			ballVelocity.y *= -1;
		}

		unsigned char reset = 0;
		if (ballOffset.x - ballRadius <= 0) {
			std::cout << "Right player point" << std::endl;
			reset = 1;
		}
		else if (ballOffset.x + ballRadius >= screenWidth) {
			std::cout << "Left player point" << std::endl;
			reset = 2;
		}

		if (reset) {
			ballOffset = { screenWidth / 2.0f, screenHeight / 2.0f };
			ballVelocity.x = reset == 1 ? initBallVelocity.x : -initBallVelocity.x;
			ballVelocity.y = initBallVelocity.y;
		}


		if (framesSinceLastCollision >= framesThreshold) {
			/*
			paddle collision
		*/
			int i = 0;
			if (ballOffset.x > screenHeight / 2.0f) {
				//if ball on right side, check with right paddle
				i++;
			}

			//get distance from enter of ball to center of paddle
			vec2 distance = { abs(ballOffset.x - paddleOffsets[i].x), abs(ballOffset.y - paddleOffsets[i].y) };

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
		updateData<vec2>(paddleVAO.offsetVBO, 0, 2, paddleOffsets);
		updateData<vec2>(ballVAO.offsetVBO, 0, 1, &ballOffset);

		//render object
		bindShader(shaderProgram);
		draw(paddleVAO, GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, 0, 2);
		draw(ballVAO, GL_TRIANGLES, 3 * numTriangles, GL_UNSIGNED_INT, 0);

		//swap frames
		newFrame(window);
	}

	//cleanup memory
	cleanup(paddleVAO);
	cleanup(ballVAO);
	deleteShader(shaderProgram);
	cleanup();

	return 0;
}
