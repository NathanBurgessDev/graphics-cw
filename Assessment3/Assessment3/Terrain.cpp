#include "Terrain.h"


Terrain::Terrain(GLuint shaderProgram,float width, float height, int resolution){
	this->shaderProgram = shaderProgram;
	model = std::make_shared<glm::mat4>(1.f);
	Object obj = Object();
	strcpy(obj.mtl.fil_name,"objs/floor/grassTexture.png");

	obj.model = model;

	for (int z = 0; z < resolution; z++) {
		for (int x = 0; x < resolution; x++) {
			
			vertex vertex0;
			glm::vec3 pos0 = glm::vec3();
			glm::vec3 tex0 = glm::vec3();

			pos0.x = (-width / 2.0f + width * z / (float)resolution); // v.x
			pos0.y = (0.0f); // v.y
			pos0.z = ( - height / 2.0f + height * x / (float)resolution); // v.z
			tex0.x = (z / (float)resolution); // u
			tex0.y = (x / (float)resolution); // v
			tex0.z = 0.0;

			vertex0.vc = pos0;
			vertex0.tc = tex0;

			vertex vertex1;
			glm::vec3 pos1 = glm::vec3();
			glm::vec3 tex1 = glm::vec3();

			pos1.x = (-width / 2.0f + width * (z + 1) / (float)resolution); // v.x
			pos1.y = (0.0f); // v.y
			pos1.z = (-height / 2.0f + height * x / (float)resolution); // v.z
			tex1.x = ((z + 1) / (float)resolution); // u
			tex1.y = (x / (float)resolution); // v
			tex1.z = 0.0;

			vertex1.vc = pos1;
			vertex1.tc = tex1;

			vertex vertex2;
			glm::vec3 pos2 = glm::vec3();
			glm::vec3 tex2 = glm::vec3();

			pos2.x = (-width / 2.0f + width * z / (float)resolution); // v.x
			pos2.y = (0.0f); // v.y
			pos2.z = (-height / 2.0f + height * (x + 1) / (float)resolution); // v.z
			tex2.x = (z / (float)resolution); // u
			tex2.y = ((x + 1) / (float)resolution); // v
			tex2.z = 0.0f;

			vertex2.vc = pos2;
			vertex2.tc = tex2;

			vertex vertex3;
			glm::vec3 pos3 = glm::vec3();
			glm::vec3 tex3 = glm::vec3();

			pos3.x = (-width / 2.0f + width * (z + 1) / (float)resolution); // v.x
			pos3.y = (0.0f); // v.y
			pos3.z = (-height / 2.0f + height * (x + 1) / (float)resolution); // v.z
			tex3.x = ((z + 1) / (float)resolution); // u
			tex3.y = ((x + 1) / (float)resolution); // v
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
	objs.push_back(obj);

}

//void Terrain::renderFullObject(){
//	for (Object& obj : objs) {
//		renderObject(obj);
//	}
//}
//
//void Terrain::renderObject(Object obj) {
//	glBindTexture(GL_TEXTURE_2D, obj.texture);
//	glBindVertexArray(obj.VAO);
//
//	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(*model));
//
//	glDrawArrays(GL_TRIANGLES, 0, (obj.tris.size() * 3));
//}
//
//void Terrain::setupTextureAndVAO(Object& obj) {
//	obj.texture = CreateTexture(obj.mtl.fil_name);
//
//	glGenVertexArrays(1, &obj.VAO);
//	glGenBuffers(1, &obj.VBO);
//
//	glBindVertexArray(obj.VAO);
//	glBindBuffer(GL_ARRAY_BUFFER, obj.VBO);
//
//	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (obj.tris.size() * 27), obj.tris.data(), GL_STATIC_DRAW);
//	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
//	glEnableVertexAttribArray(0);
//	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
//	glEnableVertexAttribArray(1);
//	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
//	glEnableVertexAttribArray(2);
//}