#pragma once

#include <GL/gl3w.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "shadow.h"
#include "camera.h"

class ShadowRendering
{
public:
	int width;
	int height;
	int sh_map_width = 2048;
	int sh_map_height = 2048;

	ShadowRendering(int width, int height);
	void generateDepthMap(unsigned int shadowShaderProgram, ShadowStruct shadow,
		glm::mat4 projectedLightSpaceMatrix);
	void renderWithShadow(unsigned int renderShaderProgram, ShadowStruct shadow,
		glm::mat4 projectedLightSpaceMatrix, glm::vec3 lightDirection, glm::vec3 lightPos, SCamera camera);
};

