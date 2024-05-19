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
#include "shadow.h"
#include "Terrain.h"
#include "ShadowRendering.h"



const int width = 1200;
const int height = 800;

unsigned int floorVAO;
unsigned int floorVBO;

SCamera Camera;

glm::vec3 lightDirection = glm::vec3(0.1f, -.81f, -.61f);
glm::vec3 lightPos = glm::vec3(2.f, 6.f, 7.f);



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
		Camera.cam_dist -= 0.1;
		camChanged = true;
	}

	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
	{
		Camera.cam_dist += 0.1;
		camChanged = true;
	}

	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		lightDirection = Camera.Front;
		lightPos = Camera.Position;
	}

	if (camChanged)
	{
		MoveAndOrientCamera(Camera, glm::vec3(0, 0, 0), Camera.cam_dist, xOffset, yOffset);
	}
}



int main()
{
	glfwInit();

	// Setup window + callbacks
	GLFWwindow* window = glfwCreateWindow(width, height, "Assesment 3", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetWindowSizeCallback(window, SizeCallback);

	gl3wInit();
	glfwSwapInterval(1);

	// Debugging
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(DebugCallback, 0);

	
	//ShadowRendering shadowRenderer = ShadowRendering(width, height);

	//GLuint textureShaderProgram = CompileShader("textured.vert", "textured.frag");
	//GLuint basicShaderProgram = CompileShader("basic.vert", "basic.frag");
	GLuint phongProgram = CompileShader("phong.vert", "phong.frag");
	//GLuint shadowProgram = CompileShader("shadow.vert", "shadow.frag");


	InitCamera(Camera);
	Camera.cam_dist = 5.f;
	MoveAndOrientCamera(Camera, glm::vec3(0, 0, 0), Camera.cam_dist, 0.f, 0.f);

	vector<CompleteObject> objs;
	glEnable(GL_DEPTH_TEST);

	CompleteObject tree0 = CompleteObject(phongProgram,"objs/white_oak/white_oak.obj");
	tree0.scale(0.005f, 0.005f, 0.005f);

	CompleteObject tree1 = CompleteObject(phongProgram,"objs/white_oak/white_oak.obj");
	tree1.translate(5.0f, 0.0f, 0.0f);
	tree1.scale(0.005f, 0.005f, 0.005f);
	
	//Terrain ground
	Terrain ground = Terrain(phongProgram, 20,20, 40,10);
	
	
 	objs.push_back(tree0);
	objs.push_back(tree1);
	objs.push_back(ground);

	

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindVertexArray(0);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	double prevTime = 0.0;
	double currentTime = 0.0;
	double timeDiff;
	unsigned int counter = 0;


	glEnable(GL_DEPTH_TEST);


	//glUseProgram(textureShaderProgram);

	while (!glfwWindowShouldClose(window))
	{

		currentTime = glfwGetTime();
		timeDiff = currentTime - prevTime;
		counter++;
		if (timeDiff >= 1.0 / 30.0) {
			std::string FPS = std::to_string((1.0 / timeDiff) * counter);
			std:string ms = std::to_string((timeDiff / counter) * 1000);
			std::string newTitle = "Assesment3 - " + FPS + "FPS / " + ms + "ms";
			glfwSetWindowTitle(window, newTitle.c_str());
			prevTime = currentTime;
			counter = 0;
			//std::cout << newTitle << std::endl;

		}

		float near_plane = 1;
		float far_plane = 70.5;

		glClearColor(.8f, .8f, .8f, 1.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		


		/*glm::mat4 lightProjection = glm::ortho(-10.f, 10.f, -10.f, 10.f, near_plane, far_plane);
		glm::mat4 lightView = glm::lookAt(lightPos, lightPos + lightDirection, glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 projectedLightSpaceMatrix = lightProjection * lightView;
*/

		// ~~~~~~ Lighting ~~~~~~~




		/*glUseProgram(basicShaderProgram);
		glUniformMatrix4fv(glGetUniformLocation(basicShaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(basicShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		drawFloor(basicShaderProgram);*/

		

		glUseProgram(phongProgram);

		

		glUniform3f(glGetUniformLocation(phongProgram, "lightDirection"), lightDirection.x, lightDirection.y, lightDirection.z);
		glUniform3f(glGetUniformLocation(phongProgram, "lightColour"), 1.f, 1.f, 1.f);
		glUniform3f(glGetUniformLocation(phongProgram, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(phongProgram, "camPos"), Camera.Position.x, Camera.Position.y, Camera.Position.z);
	

		glm::mat4 view = glm::mat4(1.f);
		glm::mat4 projection = glm::mat4(1.f);

		view = glm::lookAt(Camera.Position, Camera.Position + Camera.Front, Camera.Up);

		glUniformMatrix4fv(glGetUniformLocation(phongProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

		projection = glm::perspective(glm::radians(45.f), (float)width / (float)height, .01f, 100.f);
		glUniformMatrix4fv(glGetUniformLocation(phongProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		for (int i = 0; i < objs.size();i++) {
			objs[i].renderFullObject();
		}


		glfwSwapBuffers(window);
		glfwPollEvents();
		processKeyboard(window);
	}
	glfwDestroyWindow(window);
	glfwTerminate();
}