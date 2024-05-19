#pragma once

#include "bitmap.h"

struct ShadowStruct
{
	unsigned int FBO;
	unsigned int Texture;
};

ShadowStruct setup_shadowmap(int w, int h);
void saveShadowMapToBitmap(unsigned int Texture, int w, int h);