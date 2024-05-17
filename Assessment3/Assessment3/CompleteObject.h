#pragma once

#include "obj.h"


class CompleteObject
{	
public:
	vector<Object> objs;
	std::shared_ptr<glm::mat4> model;

	CompleteObject(const char* filename);

	void renderFullObject(unsigned int shaderProgram);

	void translate(float x, float y, float z);

	void scale(float x, float y, float z);

};