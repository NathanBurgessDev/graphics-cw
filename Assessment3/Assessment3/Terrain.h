#pragma once
#include "CompleteObject.h"
class Terrain : public CompleteObject
{
public:
	Terrain(GLuint shaderProgram, float width, float height, float resolution, float res);
	//void renderFullObject();
	void setupTextureAndVAO(Object& obj);
	//void renderObject(Object obj);
};

