#include "init.hh"
#include "vao.hh"
#include <string>
#include <sstream>
#include <fstream>

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
	glViewport(0, 0, width, height);
	::width = width;
	::height = height;

	//update project matrix
	setOrthographicProjection(shaderProgram, 0, width, 0, height, 0.0f, 1.0f);
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
	GLuint shaderProgram = glCreateProgram();

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
void bindShader(GLuint shaderProgram) {
	glUseProgram(shaderProgram);
}

//set projection
void setOrthographicProjection(GLuint shaderProgram,
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
void deleteShader(GLuint shaderProgram) {
	glDeleteProgram(shaderProgram);
}

/*
	main loop methods
*/

//process input
void processInput(GLFWwindow* window, float* offset) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		offset[1] += 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		offset[1] -= 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		offset[0] += 1.0f;
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		offset[0] -= 1.0f;
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