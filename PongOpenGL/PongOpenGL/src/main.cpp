#include <string>
#include <sstream>
#include <fstream>
#include "init.hh"
#include "vao.hh"

unsigned int width = 800;
unsigned int height = 600;
const char* title = "Pong";

int main() {
	std::cout << "Initializing Window" << std::endl;

	//timing
	double dt = 0.0;
	double lastFrame = 0.0;

	//initialization
	initGLFW(3, 3);

	//create window
	GLFWwindow* window = nullptr;
	createWindow(window, title, width, height, framebufferSizeCallback);
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

	glViewport(0, 0, width, height);

	//shaders
	GLuint shaderProgram = genShaderProgram("assets/main.vs", "assets/main.fs");
	setOrthographicProjection(shaderProgram, 0, width, 0, height, 0.0f, 1.0f);

	//setup vertex data
	float vertices[] = {
		0.5f, 0.5f,
		-0.5f, 0.5f,
		-0.5f, -0.5f,
		0.5f, -0.5f
	};

	//setup index data
	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	float offsets[] = {
		200.0f, 200.0f
	};

	float sizes[] = {
		50.0f, 50.0f
	};

	//setup VAO/VBOs
	VAO vao;
	genVAO(&vao);

	//pos VBO
	genBufferObject<float>(vao.posVBO, GL_ARRAY_BUFFER, 2 * 4, vertices, GL_STATIC_DRAW);
	setAttPointer<float>(vao.posVBO, 0, 2, GL_FLOAT, 2, 0);

	// offset VBO
	genBufferObject<float>(vao.offsetVBO, GL_ARRAY_BUFFER, 1 * 2, offsets, GL_DYNAMIC_DRAW);
	setAttPointer<float>(vao.offsetVBO, 1, 2, GL_FLOAT, 2, 0, 1);

	//size VBO
	genBufferObject<float>(vao.sizeVBO, GL_ARRAY_BUFFER, 1 * 2, offsets, GL_DYNAMIC_DRAW);
	setAttPointer<float>(vao.sizeVBO, 2, 2, GL_FLOAT, 2, 0, 1);

	//EBO
	genBufferObject<unsigned int>(vao.EBO, GL_ELEMENT_ARRAY_BUFFER, 3 * 2, indices, GL_STATIC_DRAW);

	//unbind VBO and VAO
	unbindBuffer(GL_ARRAY_BUFFER);
	unbindVAO();

	//render loop
	while (!glfwWindowShouldClose(window)) {
		//update time
		dt = glfwGetTime() - lastFrame;
		lastFrame += dt;

		//input
		processInput(window, offsets);

		//clear screen for new frame
		clearScreen();

		//update
		updateData<float>(vao.offsetVBO, 0, 1 * 2, offsets);

		//render object
		bindShader(shaderProgram);
		draw(vao, GL_TRIANGLES, 3 * 2, GL_UNSIGNED_INT, 0);

		//swap frames
		newFrame(window);
	}

	//cleanup memory
	cleanup(vao);
	deleteShader(shaderProgram);
	cleanup();

	return 0;
}
