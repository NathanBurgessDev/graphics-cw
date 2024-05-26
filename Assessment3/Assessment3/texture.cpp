#include "texture.h"

GLuint CreateTexture(const char* filename)
{

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	GLuint texObject;
	glGenTextures(1, &texObject);
	glBindTexture(GL_TEXTURE_2D, texObject);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);



	int width, height, channels;
	unsigned char* pxls = stbi_load(filename, &width, &height, &channels, 0);

	stbi_set_flip_vertically_on_load(true);
	if (pxls != NULL)
	{
		printf("Loaded %s\n", filename);
		if (channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pxls);
		if (channels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, pxls);
	}
	else
	{
		printf("Failed to load %s\n", filename);
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	delete[] pxls;

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	return texObject;
}

GLuint loadCubeMap(std::vector<std::string> faces) {

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

std::vector<std::vector<unsigned char>> getHeightMapAs2DArray(std::string filePath, int& width, int& height, int& numChannels) {
	// Load image data
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &numChannels, 0);
	if (!data) {
		std::cerr << "Failed to load image: " << filePath << std::endl;
		return {};
	}

	// Create a 2D array
	std::vector<std::vector<unsigned char>> image(height, std::vector<unsigned char>(width * numChannels));

	// Copy image data to 2D array
	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width * numChannels; ++x) {
			image[y][x] = data[y * width * numChannels + x];
		}
	}

	// Free the image memory
	stbi_image_free(data);

	return image;
}

void freeImage(unsigned char* data) {
	stbi_image_free(data);
}
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
