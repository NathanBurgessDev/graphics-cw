#include <GL/gl3w.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <memory>

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
#include "Tree.h"
#include "SkyBox.h"


int width = 1200;
int height = 800;

unsigned int floorVAO;
unsigned int floorVBO;
// Timing stuff
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// Camera stuff
SCamera camera = SCamera();
float lastX = width / 2.0f;
float lastY = height / 2.0f;
bool firstMouse = true;


glm::vec3 lightDirection = glm::vec3(0.1f, -.81f, -.61f);
glm::vec3 lightPos = glm::vec3(2.f, 6.f, 7.f);



void SizeCallback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
	width = w;
	height = h;
	lastX = width / 2.0f;
	lastY = height / 2.0f;


	ShadowRendering* shadowRenderer = static_cast<ShadowRendering*>(glfwGetWindowUserPointer(window));
	shadowRenderer->height = h;
	shadowRenderer->width = w;
}


void processKeyboard(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);


	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}



int main()
{
	glfwInit();


	glfwWindowHint(GLFW_SAMPLES, 32);
	// Setup window + callbacks
	GLFWwindow* window = glfwCreateWindow(width, height, "Assesment 3", NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetWindowSizeCallback(window, SizeCallback);



	gl3wInit();
 	glfwSwapInterval(1);

	// Debugging
	glEnable(GL_DEBUG_OUTPUT);
	glDebugMessageCallback(DebugCallback, 0);

	
	

	//GLuint textureShaderProgram = CompileShader("textured.vert", "textured.frag");
	//GLuint basicShaderProgram = CompileShader("basic.vert", "basic.frag");
	GLuint phongProgram = CompileShader("phong.vert", "phong.frag");
	GLuint shadowProgram = CompileShader("shadow.vert", "shadow.frag");
	GLuint skyBoxProgram = CompileShader("skybox.vert", "skybox.frag");
	GLuint heightMapProgram = CompileShader("heightMap.vert", "heightMap.frag");

	


	
	camera.cam_dist = 5.f;

	SkyBox skyBox = SkyBox(skyBoxProgram);

	vector<std::unique_ptr<CompleteObject>> objs;
	glEnable(GL_DEPTH_TEST);

	std::unique_ptr<Tree> tree0 = std::make_unique<Tree>(phongProgram,"objs/white_oak/white_oak.obj");
	tree0->translate(0.f, 2.f, 0.0f);
	tree0->scale(0.005f, 0.005f, 0.005f);

	std::unique_ptr<Tree> tree1 = std::make_unique<Tree>(phongProgram, "objs/white_oak/white_oak.obj");
	tree1->translate(5.0f, 2.f, 0.0f);
	tree1->scale(0.005f, 0.005f, 0.005f);
	
	//Terrain ground

	std::unique_ptr<CompleteObject> f22 = std::make_unique<CompleteObject>(phongProgram, "objs/f22/F-22.obj");
	f22->translate(-8.f, 5.f, 0.f);
	f22->scale(0.05f, 0.05f, 0.05f);


	std::unique_ptr<Terrain> ground= std::make_unique<Terrain>(heightMapProgram, 100,100, 40,1.0);
	
	objs.push_back(std::move(ground));
	objs.push_back(std::move(f22));
 	objs.push_back(std::move(tree0));
	objs.push_back(std::move(tree1));
	
	

	ShadowRendering shadowRenderer = ShadowRendering(width, height);
	glfwSetWindowUserPointer(window, &shadowRenderer);

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
		deltaTime = static_cast<float>(currentTime) - lastFrame;
		timeDiff = currentTime - prevTime;

		lastFrame = static_cast<float>(currentTime);
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
		processKeyboard(window);

		float near_plane = 1.f;
		float far_plane = 75.5f;


		// ~~~~~~~ Generate ShadowDepthMap ~~~~~~~~~~

		glm::mat4 lightProjection = glm::ortho(-10.f, 10.f, -10.f, 10.f, near_plane, far_plane);
		glm::mat4 lightView = glm::lookAt(lightPos, lightPos + lightDirection, glm::vec3(0.f, 1.f, 0.f));
		glm::mat4 projectedLightSpaceMatrix = lightProjection * lightView;

		shadowRenderer.generateDepthMap(shadowProgram, objs, projectedLightSpaceMatrix);
		//shadowRenderer.saveShadowMap();
		

		//~~~~~~~ End ShadowDepth Map ~~~~~~~~~~~~~~~

		glViewport(0, 0, width, height);
		glClearColor(.8f, .8f, .8f, .8f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


		// ~~~~~~ Lighting + Shadows ~~~~~~~
	
		glm::mat4 view = glm::mat4(1.f);
		glm::mat4 projection = glm::mat4(1.f);

		view = glm::lookAt(camera.Position, camera.Position + camera.Front, camera.Up);
		projection = glm::perspective(glm::radians(45.f), (float)width / (float)height, .01f, 300.f);

		skyBox.RenderSkyBox(view,projection);


		glUseProgram(phongProgram);

	/*	GLuint shadowPosPhong = glGetUniformLocation(phongProgram, "shadowMap");
		GLuint shadowPosTerrain = glGetUniformLocation(heightMapProgram, "shadowMap");

		glUniform1i(shadowPosPhong, 1);
		glUniform1i(shadowPosTerrain, 2);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, shadowRenderer.shadowMap.Texture);

		glUseProgram(heightMapProgram);

		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, shadowRenderer.shadowMap.Texture);

		glUseProgram(phongProgram);*/


		
		glUniformMatrix4fv(glGetUniformLocation(phongProgram, "projectedLightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(projectedLightSpaceMatrix));
		glUniform3f(glGetUniformLocation(phongProgram, "lightDirection"), lightDirection.x, lightDirection.y, lightDirection.z);
		glUniform3f(glGetUniformLocation(phongProgram, "lightColour"), 1, 1, 1);
		glUniform3f(glGetUniformLocation(phongProgram, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(phongProgram, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);

		glm::vec3 lightColour = glm::vec3(1.f, 1.f, 1.f);
		glm::vec3 diffColour = lightColour * glm::vec3(1.f);
		glm::vec3 ambColour = diffColour * glm::vec3(1.f);

		glUniform3fv(glGetUniformLocation(phongProgram, "lightAmb"), 1, glm::value_ptr(ambColour));
		glUniform3fv(glGetUniformLocation(phongProgram, "lightDiff"),1,glm::value_ptr(diffColour));
		glUniform3f(glGetUniformLocation(phongProgram, "lightSpec"), 1.f, 1.f, 1.f);

		//view = glm::lookAt(Camera.Position, Camera.Position + Camera.Front, Camera.Up);

		glUniformMatrix4fv(glGetUniformLocation(phongProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

		//projection = glm::perspective(glm::radians(45.f), (float)width / (float)height, .01f, 100.f);
		glUniformMatrix4fv(glGetUniformLocation(phongProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));



		glUseProgram(heightMapProgram);

		glUniformMatrix4fv(glGetUniformLocation(heightMapProgram, "projectedLightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(projectedLightSpaceMatrix));
		glUniform3f(glGetUniformLocation(heightMapProgram, "lightDirection"), lightDirection.x, lightDirection.y, lightDirection.z);
		glUniform3f(glGetUniformLocation(heightMapProgram, "lightColour"), 1, 1, 1);
		glUniform3f(glGetUniformLocation(heightMapProgram, "lightPos"), lightPos.x, lightPos.y, lightPos.z);
		glUniform3f(glGetUniformLocation(heightMapProgram, "camPos"), camera.Position.x, camera.Position.y, camera.Position.z);
	
		//view = glm::lookAt(Camera.Position, Camera.Position + Camera.Front, Camera.Up);

		glUniformMatrix4fv(glGetUniformLocation(heightMapProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

		//projection = glm::perspective(glm::radians(45.f), (float)width / (float)height, .01f, 100.f);

		glUniformMatrix4fv(glGetUniformLocation(heightMapProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

		//glEnable(GL_FRAMEBUFFER_SRGB);

		for (std::unique_ptr<CompleteObject>& obj: objs) {
			obj->renderFullObject(shadowRenderer.shadowMap.Texture);
		}

		//glDisable(GL_FRAMEBUFFER_SRGB);
		

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
}