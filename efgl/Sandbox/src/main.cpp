#include <efgl.h>
#include <GLFW/glfw3.h>

#include <string>
#include <cmath>
#include <iostream>

using namespace std;
using namespace efgl;
using namespace efgl::ogl;

int main() {

	GLwindow* window = GLwindow::getWindow();
	window->init(800, 600, "Hello window");

	float vertices[] = {
	 0.0f,  0.5f, 0.0f, // top
	 0.5f, -0.5f, 0.0f, // bottom right
	-0.5f, -0.5f, 0.0f  // bottom left
	};

	unsigned int indices[] = {
		0, 1, 2
	};

	IndexBuffer ib(indices, 3);

	Shader shader("src/shaders/shader.glsl");
	shader.bind();

	VertexBuffer vb(vertices, sizeof(vertices));

	VertexBufferLayout vbl;
	vbl.push<float>(3, false);
	VertexArray vao;
	vao.addBuffer(vb, vbl);

	ib.bind();

	while (!window->shouldClose())
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.bind();
		vao.bind();
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

		window->swap();
	}

	return 0;
}