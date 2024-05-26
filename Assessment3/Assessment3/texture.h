//#pragma once
//
//#include "stb_image.h"
//
//GLuint setupTexture(const char* filename)
//{
//	glEnable(GL_TEXTURE_2D);
//	glEnable(GL_BLEND);
//
//	GLuint texObject;
//	glGenTextures(1, &texObject);
//	glBindTexture(GL_TEXTURE_2D, texObject);
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//
//
//	int w, h, chan;
//	stbi_set_flip_vertically_on_load(true);
//	unsigned char* pxls = stbi_load(filename, &w, &h, &chan, 0);
//
//	if (pxls != NULL) {
//		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pxls);
//	}
//
//	glGenerateMipmap(GL_TEXTURE_2D);
//
//	delete[] pxls;
//	glDisable(GL_TEXTURE_2D);
//	glDisable(GL_BLEND);
//
//	return texObject;
//}
//
//GLuint setup_mipmaps(const char* filename[], int n){
//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//
//	int w[16], h[16], chan[16];
//	unsigned char* pxls[16];
//	stbi_set_flip_vertically_on_load(true);
//	for (int c = 0; c < n; c++) {
//		pxls[c] = stbi_load(filename[c], &w[c], &h[c], &chan[c], 0);
//		if (pxls[c]) {
//			glTexImage2D(GL_TEXTURE_2D, c, GL_RGB, w[c], h[c], 0, GL_RGB, GL_UNSIGNED_BYTE, pxls[c]);
//		}
//		delete pxls[c];
//	}
//
//	return 0;
//}


#pragma once

#include <GL/gl3w.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "stb_image.h"

#include <vector>
#include <string>
#include <iostream>

GLuint CreateTexture(const char* filename);
GLuint loadCubeMap(std::vector<std::string> faces);
std::vector<std::vector<unsigned char>> getHeightMapAs2DArray(std::string filePath, int& width, int& height, int& numChannels);
void freeImage(unsigned char*);
