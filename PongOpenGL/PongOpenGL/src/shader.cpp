#include "shader.hpp"

//Read File
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

//Generate Shader
GLuint genShader(const char* filepath, GLenum type) {
	std::string shaderSrc = readFile(filepath);
	const GLchar* shader = shaderSrc.c_str();

	//build and compile the shader
	GLuint shaderInt = glCreateShader(type);
	glShaderSource(shaderInt, 1, &shader, NULL);
	glCompileShader(shaderInt);

	//check for errors
	int success;
	char infoLog[512];
	glGetShaderiv(shaderInt, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderInt, 512, NULL, infoLog);
		std::cout << "Error in shader compilation:" << std::endl << infoLog << std::endl;
		throw(errno);
	}

	return shaderInt;
}

GLuint genShaderString(std::string filestring, GLenum type) {
	const GLchar* shader = filestring.c_str();
	std::cout << "Shader:" << std::endl << shader << std::endl;

	//build and compile the shader
	GLuint shaderInt = glCreateShader(type);
	glShaderSource(shaderInt, 1, &shader, NULL);
	glCompileShader(shaderInt);

	//check for errors
	int success;
	char infoLog[512];
	glGetShaderiv(shaderInt, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderInt, 512, NULL, infoLog);
		std::cout << "Error in shader compilation:" << std::endl << infoLog << std::endl;
		throw(errno);
	}
	if (shaderInt == NULL) {
		throw(errno);
	}
	return shaderInt;
}


/*
	SHADER CLASS
*/
Shader::Shader(const char* vertexShaderFile, const char* fragmentShaderFile) {
	shaderObj = glCreateProgram();

	//compile shaders
	GLuint vertexShader = genShader(vertexShaderFile, GL_VERTEX_SHADER);
	GLuint fragmentShader = genShader(fragmentShaderFile, GL_FRAGMENT_SHADER);

	if (vertexShader == -1 || fragmentShader == -1) {
		throw(errno);
	}

	//link shader
	glAttachShader(shaderObj, vertexShader);
	glAttachShader(shaderObj, fragmentShader);
	glLinkProgram(shaderObj);

	//check for errors
	int success;
	char infoLog[512];
	glGetShaderiv(shaderObj, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderObj, 512, NULL, infoLog);
		std::cout << "Error in shader linking:" << std::endl << infoLog << std::endl;
		throw(errno);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

Shader::Shader(std::string vertexShaderFile, std::string fragmentShaderFile) {
	shaderObj = glCreateProgram();

	//compile shaders
	GLuint vertexShader = genShaderString(vertexShaderFile, GL_VERTEX_SHADER);
	GLuint fragmentShader = genShaderString(fragmentShaderFile, GL_FRAGMENT_SHADER);

	if (vertexShader == -1 || fragmentShader == -1) {
		throw(errno);
	}

	//link shader
	glAttachShader(shaderObj, vertexShader);
	glAttachShader(shaderObj, fragmentShader);
	glLinkProgram(shaderObj);

	//check for errors
	int success;
	char infoLog[512];
	glGetShaderiv(shaderObj, GL_LINK_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderObj, 512, NULL, infoLog);
		std::cout << "Error in shader linking:" << std::endl << infoLog << std::endl;
		throw(errno);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}

void Shader::Activate() {
	glUseProgram(shaderObj);
}

void Shader::Delete() {
	glDeleteProgram(shaderObj);
}