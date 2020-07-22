#include "SpinningPyramid.h"

#include <efgl.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <cmath>
#include <iostream>

using namespace std;
using namespace efgl;
using namespace efgl::ogl;

void makeSierpinski(Shader shader, float x, float y, float scl, unsigned int depth)
{
	static unsigned int minDepth = 1;
	if (depth < minDepth) return;

	glm::mat4 ROTATION = glm::mat4(1.0f);
	ROTATION = glm::rotate(ROTATION, depth * (float)glm::radians(glfwGetTime()), glm::vec3(0.0, 1.0, 0.0));

	glm::vec3 offset = glm::vec3(0.0f, 0.5f, 0.0f) * scl;
	glm::mat4 trans1 = glm::mat4(1.0f);
	trans1 = ROTATION * trans1;
	trans1 = glm::translate(trans1, offset);
	trans1 = glm::translate(trans1, glm::vec3(x, y, 0.0f));
	trans1 = glm::scale(trans1, scl * 0.5f * glm::vec3(1.0f, 1.0f, 1.0f));
	shader.bind();
	shader.setUniform("transform", trans1);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	makeSierpinski(shader, offset.x + x, offset.y + y, scl * 0.5f, depth - 1);

	offset = glm::vec3(0.5f, -0.5f, 0.0f) * scl;
	glm::mat4 trans2 = glm::mat4(1.0f);
	trans2 = ROTATION * trans2;
	trans2 = glm::translate(trans2, offset);
	trans2 = glm::translate(trans2, glm::vec3(x, y, 0.0f));
	trans2 = glm::scale(trans2, scl * 0.5f * glm::vec3(1.0f, 1.0f, 1.0f));
	shader.bind();
	shader.setUniform("transform", trans2);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	makeSierpinski(shader, offset.x + x, offset.y + y, scl * 0.5f, depth - 1);

	offset = glm::vec3(-0.5f, -0.5f, 0.0f) * scl;
	glm::mat4 trans3 = glm::mat4(1.0f);
	trans3 = ROTATION * trans3;
	trans3 = glm::translate(trans3, offset);
	trans3 = glm::translate(trans3, glm::vec3(x, y, 0.0f));
	trans3 = glm::scale(trans3, scl * 0.5f * glm::vec3(1.0f, 1.0f, 1.0f));
	shader.bind();
	shader.setUniform("transform", trans3);
	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
	makeSierpinski(shader, offset.x + x, offset.y + y, scl * 0.5f, depth - 1);

}

void SpinningPyramid()
{
	GLwindow* window = GLwindow::init(800, 800, "Sierpinksi Triangle");

	float vertices[] = {
		// positions         // texture coords
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f, // bottom left
		 0.0f,  1.0f, 0.0f,  0.5f, 1.0f, // top middle
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f, // bottom right
	};
	
	unsigned int indices[] = {
		0, 1, 2
	};

	Texture2D texture("src/resources/awesomeface.png", "face", true);

	IndexBuffer ib(indices, 3);

	Shader shader("src/shaders/SpinningPyramid.glsl");
	shader.bind();
	shader.setUniform("tex", 0);

	VertexBuffer vb(vertices, sizeof(vertices));

	VertexBufferLayout vbl;
	vbl.push<float>(3, false);
	vbl.push<float>(2, false);
	VertexArray vao;
	vao.addBuffer(vb, vbl);

	while (!window->shouldClose())
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		texture.bind();
		shader.bind();
		vao.bind();
		ib.bind();

		makeSierpinski(shader, 0.0f, 0.0f, 1.0, 4);

		window->swap();
	}

}