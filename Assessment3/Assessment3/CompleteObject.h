#pragma once

#include "obj.h"
#include <vector>


class CompleteObject
{	
public:
	vector<Object> objs;
	std::shared_ptr<glm::mat4> model;
	GLuint shaderProgram;

	CompleteObject(GLuint shaderProgram, const char* filename);
	CompleteObject(GLuint shaderProgram);
	CompleteObject();
	//CompleteObject(std::vector<float>& verticies, std::string fileName)
	virtual void renderFullObject();
	virtual void renderFullObjectWithShader(GLuint newShaderProgram);

	void translate(float x, float y, float z);

	void scale(float x, float y, float z);

};