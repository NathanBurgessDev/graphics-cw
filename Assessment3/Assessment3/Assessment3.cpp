#include <GL/gl3w.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>

using namespace std;
#define STB_IMAGE_IMPLEMENTATION
//#include "texture.h"
#include "error.h"
#include "shader.h"
//#include "obj.h"
#include "camera.h"
#include "CompleteObject.h"

const int width = 1200;
const int height = 800;

#define NUM_BUFFERS 1
#define NUM_VAOS 1
GLuint Buffers[NUM_BUFFERS];
GLuint VAOs[NUM_VAOS];

SCamera Camera;

float vertices[] =
{
	//back face
	//pos					//col				//normal
	-0.5f, -0.5f, -0.5f,  	1.f, 1.0f, 1.0f,	 0.f, 0.f, -1.f,
	0.5f, -0.5f, -0.5f,  	1.f, 1.0f, 1.0f,	 0.f, 0.f, -1.f,
	0.5f,  0.5f, -0.5f,  	1.f, 1.0f, 1.0f,	 0.f, 0.f, -1.f,
	0.5f,  0.5f, -0.5f,  	1.f, 1.0f, 1.0f,	 0.f, 0.f, -1.f,
	-0.5f,  0.5f, -0.5f,  	1.f, 1.0f, 1.0f,	 0.f, 0.f, -1.f,
	-0.5f, -0.5f, -0.5f,  	1.f, 1.0f, 1.0f,	 0.f, 0.f, -1.f,

	//front face
	-0.5f, -0.5f,  0.5f,  	1.0f, 1.0f, 1.0f,	 0.f, 0.f, 1.f,
	0.5f, -0.5f,  0.5f,  	1.0f, 1.0f, 1.0f,	 0.f, 0.f, 1.f,
	0.5f,  0.5f,  0.5f,  	1.0f, 1.0f, 1.0f,	 0.f, 0.f, 1.f,
	0.5f,  0.5f,  0.5f,  	1.0f, 1.0f, 1.0f,	 0.f, 0.f, 1.f,
	-0.5f,  0.5f,  0.5f,  	1.0f, 1.0f, 1.0f,	 0.f, 0.f, 1.f,
	-0.5f, -0.5f,  0.5f,  	1.0f, 1.0f, 1.0f,	 0.f, 0.f, 1.f,

	//left face
	-0.5f,  0.5f,  0.5f,  	1.0f, 1.0f, 1.0f,	 -1.f, 0.f, 0.f,
	-0.5f,  0.5f, -0.5f,  	1.0f, 1.0f, 1.0f,	 -1.f, 0.f, 0.f,
	-0.5f, -0.5f, -0.5f,  	1.0f, 1.0f, 1.0f,	 -1.f, 0.f, 0.f,
	-0.5f, -0.5f, -0.5f,  	1.0f, 1.0f, 1.0f,	 -1.f, 0.f, 0.f,
	-0.5f, -0.5f,  0.5f,  	1.0f, 1.0f, 1.0f,	 -1.f, 0.f, 0.f,
	-0.5f,  0.5f,  0.5f,  	1.0f, 1.0f, 1.0f,	 -1.f, 0.f, 0.f,

	//right face
	0.5f,  0.5f,  0.5f,  	1.f, 1.0f, 1.0f,	 1.f, 0.f, 0.f,
	0.5f,  0.5f, -0.5f,  	1.f, 1.0f, 1.0f,	 1.f, 0.f, 0.f,
	0.5f, -0.5f, -0.5f, 	1.f, 1.0f, 1.0f,	 1.f, 0.f, 0.f,
	0.5f, -0.5f, -0.5f,  	1.f, 1.0f, 1.0f,	 1.f, 0.f, 0.f,
	0.5f, -0.5f,  0.5f,  	1.f, 1.0f, 1.0f,	 1.f, 0.f, 0.f,
	0.5f,  0.5f,  0.5f,  	1.f, 1.0f, 1.0f,	 1.f, 0.f, 0.f,

	//bottom face
	-0.5f, -0.5f, -0.5f,  	1.f, 1.0f, 1.0f,	 0.f, -1.f, 0.f,
	0.5f, -0.5f, -0.5f,  	1.f, 1.0f, 1.0f,	 0.f, -1.f, 0.f,
	0.5f, -0.5f,  0.5f,  	1.f, 1.0f, 1.0f,	 0.f, -1.f, 0.f,
	0.5f, -0.5f,  0.5f,  	1.f, 1.0f, 1.0f,	 0.f, -1.f, 0.f,
	-0.5f, -0.5f,  0.5f,  	1.f, 1.0f, 1.0f,	 0.f, -1.f, 0.f,
	-0.5f, -0.5f, -0.5f,  	1.f, 1.0f, 1.0f,	 0.f, -1.f, 0.f,

	//top face
	-0.5f,  0.5f, -0.5f,  	1.0f, 1.f, 1.0f,	0.f, 1.f, 0.f,
	0.5f,  0.5f, -0.5f,  	1.0f, 1.f, 1.0f,	0.f, 1.f, 0.f,
	0.5f,  0.5f,  0.5f,  	1.0f, 1.f, 1.0f,	0.f, 1.f, 0.f,
	0.5f,  0.5f,  0.5f,  	1.0f, 1.f, 1.0f,	0.f, 1.f, 0.f,
	-0.5f,  0.5f,  0.5f,  	1.0f, 1.f, 1.0f,	0.f, 1.f, 0.f,
	-0.5f,  0.5f, -0.5f, 	1.0f, 1.f, 1.0f,	0.f, 1.f, 0.f,
};


void SizeCallback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}


void processKeyboard(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	float xOffset = 0;
	float yOffset = 0;
	bool camChanged = false;

	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
	{
		xOffset = 1;
		yOffset = 0;
		camChanged = true;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
	{
		xOffset = -1;
		yOffset = 0;
		camChanged = true;
	}

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
	{
		xOffset = 0;
		yOffset = 1;
		camChanged = true;
	}

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
	{
		xOffset = 0;
		yOffset = -1;
		camChanged = true;
	}

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
	{
		cam_dist -= 0.1;
		camChanged = true;
	}

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		cam_dist += 0.1;
		camChanged = true;
	}

	/*if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		lightDirection = Camera.Front;
		lightPos = Camera.Position;
	}*/

	if (camChanged)
	{
		MoveAndOrientCamera(Camera, glm::vec3(0, 0, 0), cam_dist, xOffset, yOffset);
	}
}

void drawFloor(unsigned int shaderProgram)
{
	glBindVertexArray(VAOs[0]);

	// floor
	glm::mat4 model = glm::mat4(1.f);
	model = glm::translate(model, glm::vec3(0, -3, 0));
	model = glm::scale(model, glm::vec3(100, 0.1, 100));
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glDrawArrays(GL_TRIANGLES, 0, 36);
}


int main()
{
	glfwInit();

	// Setup window + callbacks
	GLFWwindow* window = glfwCreateWindow(width,height, "Assesment 3", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, SizeCallback);

	gl3wInit();

	// Debugging
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(DebugCallback, 0);

	unsigned int textureShaderProgram = CompileShader("textured.vert", "textured.frag");
	GLuint basicShaderProgram = CompileShader("basic.vert", "basic.frag");


	InitCamera(Camera);
	cam_dist = 5.f;
	MoveAndOrientCamera(Camera, glm::vec3(0, 0, 0), cam_dist, 0.f, 0.f);

	vector<CompleteObject> objs;
	
 	objs.push_back(CompleteObject("objs/white_oak/white_oak.obj"));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);



	/*glCreateBuffers(NUM_BUFFERS, Buffers);
	glNamedBufferStorage(Buffers[0], sizeof(vertices), vertices, 0);
	glGenVertexArrays(NUM_VAOS, VAOs);
	glBindVertexArray(VAOs[0]);
	glBindBuffer(GL_ARRAY_BUFFER, Buffers[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (9 * sizeof(float)), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (9 * sizeof(float)), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, (9 * sizeof(float)), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);*/

	//glEnable(GL_DEPTH_TEST);

	glUseProgram(textureShaderProgram);

	
	while (!glfwWindowShouldClose(window))
	{
		processKeyboard(window);

		glClearColor(1.f, 1.f, 1.f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		glm::mat4 view = glm::mat4(1.f);
		view = glm::lookAt(Camera.Position, Camera.Position + Camera.Front, Camera.Up);
		glUniformMatrix4fv(glGetUniformLocation(textureShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

		glm::mat4 projection = glm::mat4(1.f);
		projection = glm::perspective(glm::radians(45.f), (float)width / (float)height, .01f, 100.f);
		glUniformMatrix4fv(glGetUniformLocation(textureShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));


		for (CompleteObject object : objs) {
			object.renderFullObject(textureShaderProgram);
		}
		
		glBindVertexArray(0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
}
