#include "Terrain.h"


Terrain::Terrain(GLuint shaderProgram,float width, float height, float resolution, float res){
	//constructFlatPlane(shaderProgram, width, height, resolution, res);
	constructHeightMapTerrain(shaderProgram, resolution, res);
}

void Terrain::constructHeightMapTerrain(GLuint shaderProgram, float resolution, float res){
	this->shaderProgram = shaderProgram;
	model = std::make_shared<glm::mat4>(1.f);
	unsigned char* data = getHeightMap("objs/floor/heightMap.png", width, height, numChannels);
	if (data)
	{
		std::cout << "Loaded heightmap of size " << height << " x " << width << std::endl;
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	// vertex generation
	std::vector<float> vertices;
	float yScale = 64.0f / 256.0f, yShift = 16.0f;
	int rez = 1;
	unsigned bytePerPixel = numChannels;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			unsigned char* pixelOffset = data + (j + width * i) * bytePerPixel;
			unsigned char y = pixelOffset[0];

			// vertex
			vertices.push_back(-height / 2.0f + height * i / (float)height);   // vx
			vertices.push_back((int)y * yScale - yShift);   // vy
			vertices.push_back(-width / 2.0f + width * j / (float)width);   // vz
		}
	}
	stbi_image_free(data);

	std::vector<unsigned> indices;
	for (unsigned i = 0; i < height - 1; i += rez)
	{
		for (unsigned j = 0; j < width; j += rez)
		{
			for (unsigned k = 0; k < 2; k++)
			{
				indices.push_back(j + width * (i + k * rez));
			}
		}
	}

	numStrips = (height - 1) / rez;
	numTrisPerStrip = (width / rez) * 2 - 2;

	
	glGenVertexArrays(1, &terrainVAO);
	glBindVertexArray(terrainVAO);

	glGenBuffers(1, &terrainVBO);
	glBindBuffer(GL_ARRAY_BUFFER, terrainVBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &terrainIBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, terrainIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned), &indices[0], GL_STATIC_DRAW);

}

void Terrain::constructFlatPlane(GLuint shaderProgram, float width, float height, float resolution, float res) {
	this->shaderProgram = shaderProgram;
	model = std::make_shared<glm::mat4>(1.f);
	Object obj = Object();
	strcpy(obj.mtl.fil_name, "objs/floor/grassTexture.png");

	obj.model = model;

	float texRes = resolution / res;

	for (unsigned i = 0; i < resolution - 1; i++) {
		for (unsigned j = 0; j < resolution - 1; j++) {

			vertex vertex0;
			glm::vec3 pos0 = glm::vec3();
			glm::vec3 tex0 = glm::vec3();

			pos0.x = (-width / 2.0f + width * i / (float)resolution); // v.x
			pos0.y = (0.0f); // v.y
			pos0.z = (-height / 2.0f + height * j / (float)resolution); // v.z
			tex0.x = (i / (float)texRes); // u
			tex0.y = (j / (float)texRes); // v
			tex0.z = 0.0;

			vertex0.vc = pos0;
			vertex0.tc = tex0;

			vertex vertex1;
			glm::vec3 pos1 = glm::vec3();
			glm::vec3 tex1 = glm::vec3();

			pos1.x = (-width / 2.0f + width * (i + 1) / (float)resolution); // v.x
			pos1.y = (0.0f); // v.y
			pos1.z = (-height / 2.0f + height * j / (float)resolution); // v.z
			tex1.x = ((i + 1) / (float)texRes); // u
			tex1.y = (j / (float)texRes); // v
			tex1.z = 0.0;

			vertex1.vc = pos1;
			vertex1.tc = tex1;

			vertex vertex2;
			glm::vec3 pos2 = glm::vec3();
			glm::vec3 tex2 = glm::vec3();

			pos2.x = (-width / 2.0f + width * i / (float)resolution); // v.x
			pos2.y = (0.0f); // v.y
			pos2.z = (-height / 2.0f + height * (j + 1) / (float)resolution); // v.z
			tex2.x = (i / (float)texRes); // u
			tex2.y = ((j + 1) / (float)texRes); // v
			tex2.z = 0.0f;

			vertex2.vc = pos2;
			vertex2.tc = tex2;

			vertex vertex3;
			glm::vec3 pos3 = glm::vec3();
			glm::vec3 tex3 = glm::vec3();

			pos3.x = (-width / 2.0f + width * (i + 1) / (float)resolution); // v.x
			pos3.y = (0.0f); // v.y
			pos3.z = (-height / 2.0f + height * (j + 1) / (float)resolution); // v.z
			tex3.x = ((i + 1) / (float)texRes); // u
			tex3.y = ((j + 1) / (float)texRes); // v
			tex3.z = 0.0f;

			vertex3.vc = pos3;
			vertex3.tc = tex3;



			vertex0.nc = glm::normalize(glm::cross((pos2 - pos0), (pos1 - pos0)));

			vertex1.nc = glm::normalize(glm::cross((pos0 - pos1), (pos2 - pos1)));

			vertex2.nc = glm::normalize(glm::cross((pos1 - pos2), (pos0 - pos2)));

			vertex3.nc = glm::normalize(glm::cross((pos1 - pos3), (pos2 - pos3)));


			obj.tris.push_back(triangle(vertex0, vertex1, vertex2));
			obj.tris.push_back(triangle(vertex1, vertex2, vertex3));
		}
	}


	setupTextureAndVAO(obj);
	obj.hasInside = false;
	objs.push_back(obj);
}

void Terrain::renderFullObject(){
	/*for (Object& obj : objs) {
		renderObject(obj);
	}*/

	glUseProgram(shaderProgram);
	glBindVertexArray(terrainVAO);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(*model));
	unsigned strip = 1;
 	for (strip = 0; strip < numStrips; strip++)
	{
		glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
			numTrisPerStrip + 2,   // number of indices to render
			GL_UNSIGNED_INT,     // index data type
			(void*)(sizeof(unsigned int) * (numTrisPerStrip + 2) * strip)); // offset to starting index
	}
}

void Terrain::renderFullObjectWithShader(GLuint newShaderProgram) {
	glUseProgram(newShaderProgram);
	glBindVertexArray(terrainVAO);
	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(*model));
	unsigned strip = 1;
	for (strip = 0; strip < numStrips; strip++)
	{
		glDrawElements(GL_TRIANGLE_STRIP,   // primitive type
			numTrisPerStrip + 2,   // number of indices to render
			GL_UNSIGNED_INT,     // index data type
			(void*)(sizeof(unsigned int) * (numTrisPerStrip + 2) * strip)); // offset to starting index
	}
}

void Terrain::renderObject(Object& obj) {
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, obj.texture);
	glBindVertexArray(obj.VAO);

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(*model));

	glDrawArrays(GL_TRIANGLES, 0, (obj.tris.size() * 3));
}

void Terrain::setupTextureAndVAO(Object& obj) {
	obj.texture = CreateTexture(obj.mtl.fil_name);

	glGenVertexArrays(1, &obj.VAO);
	glGenBuffers(1, &obj.VBO);

	glBindVertexArray(obj.VAO);
	glBindBuffer(GL_ARRAY_BUFFER, obj.VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (obj.tris.size() * 27), obj.tris.data(), GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}