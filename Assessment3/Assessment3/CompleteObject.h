#pragma once

#include "obj.h"


class CompleteObject
{	
public:
	vector<Object> objs;
	glm::mat4 model = glm::mat4(1.f);

	CompleteObject(const char* filename);

	void renderFullObject(unsigned int shaderProgram);

};