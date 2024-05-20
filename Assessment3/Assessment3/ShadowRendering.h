#pragma once

#include <GL/gl3w.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <vector>

#include "shadow.h"
#include "camera.h"
#include "CompleteObject.h"


class ShadowRendering
{
public:
	int width;
	int height;
	int sh_map_width = 2048;
	int sh_map_height = 2048;
	ShadowStruct shadowMap = setup_shadowmap(sh_map_width, sh_map_height);

	ShadowRendering(int width, int height);
	void generateDepthMap(unsigned int shadowShaderProgram, std::vector<CompleteObject>& objs,
		glm::mat4 projectedLightSpaceMatrix);
	void saveShadowMap();
};

