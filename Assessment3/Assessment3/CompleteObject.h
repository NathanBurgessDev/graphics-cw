#pragma once

#include "obj.h"
#include <vector>


class CompleteObject
{	
public:
	vector<Object> objs;
	std::shared_ptr<glm::mat4> model;
	GLuint shaderProgram;


	glm::vec3 pos = glm::vec3(0,0,0);
	glm::vec3 scale = glm::vec3(1, 1, 1);
	glm::vec3 front = glm::vec3(0, 0, -1);
	glm::vec3 up = glm::vec3(0, 1, 0);
	glm::vec3 worldUp = up;
	glm::vec3 right = glm::normalize(glm::cross(front, worldUp));

	std::optional<glm::vec3> directionVector;

	CompleteObject(GLuint shaderProgram, const char* filename);
	CompleteObject(GLuint shaderProgram);
	CompleteObject();
	//CompleteObject(std::vector<float>& verticies, std::string fileName)
	virtual void renderFullObject(GLuint shadowTexture);
	virtual void renderFullObjectWithShader(GLuint newShaderProgram);

	virtual void handleMovement(float currentTime, float deltaTime);

	glm::mat4 calcMovement();
	void stackPosition(glm::vec3 translation);
	void setStackPosition(glm::vec3 translation);

	void setPos(float x, float y, float z);

	void setScale(float x, float y, float z);

};