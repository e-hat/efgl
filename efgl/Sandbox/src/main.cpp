#include <efgl.h>
#include <GLFW/glfw3.h>

#include <string>
#include <cmath>
#include <iostream>

using namespace std;
using namespace efgl;
using namespace efgl::ogl;

int main() {

	GLwindow* window = GLwindow::init(800, 600, "Hello window");

	float vertices[] = {
		// positions          // colors           // texture coords
		 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
	};

	unsigned int indices[] = {
		0, 1, 2,
		3, 0, 2
	};

	Texture2D texture1("src/resources/container.jpg", "container");
	Texture2D texture2("src/resources/awesomeface.png", "face", true);

	IndexBuffer ib(indices, 6);

	Shader shader("src/shaders/shader.glsl");
	shader.bind();
	shader.setUniform("texture1", 0);
	shader.setUniform("texture2", 1);

	VertexBuffer vb(vertices, sizeof(vertices));

	VertexBufferLayout vbl;
	vbl.push<float>(3, false);
	vbl.push<float>(3, false);
	vbl.push<float>(2, false);
	VertexArray vao;
	vao.addBuffer(vb, vbl);

	ib.bind();

	while (!window->shouldClose())
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		texture1.bind(0);
		texture2.bind(1);

		shader.bind();
		vao.bind();
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		window->swap();
	}

	return 0;
}