#pragma once
#include "CompleteObject.h"
#include "texture.h"
class Terrain : public CompleteObject
{
public:
	Terrain(GLuint shaderProgram, float width, float height, float resolution, float res);
	int width, height, numChannels;
	unsigned int  numStrips, numTrisPerStrip;
	GLuint terrainVAO, terrainVBO, terrainIBO;
	GLuint texture;
	
	void renderFullObject(GLuint shadowTexture) override;
	void constructFlatPlane(GLuint shaderProgram, float width, float height, float resolution, float res);
	void constructHeightMapTerrain(GLuint shaderProgram, float resolution, float res);
	void renderFullObjectWithShader(GLuint newShaderProgram) override;
	void setupTextureAndVAO(Object& obj);
	void renderObject(Object& obj);
	glm::vec3 calculateNormal(int x, int z, unsigned char* data, unsigned bytePerPixel, int width,int height, float yScale, float yShift);
};

