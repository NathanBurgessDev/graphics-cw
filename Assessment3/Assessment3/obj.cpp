#pragma once

#include "obj.h"

void Object::renderObject(unsigned int shaderProgram) {
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);
	
	model = glm::scale((model), glm::vec3(.005f, .005f, .005f));

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	glDrawArrays(GL_TRIANGLES, 0, (tris.size() * 3));
}

 void Object::setupTextureAndVAO() {
	texture = CreateTexture(mtl.fil_name);

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * (tris.size() * 18), tris.data(), GL_STATIC_DRAW);
 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	/*glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(3);*/
}

int mtl_parse(char* filename, vector<Material>* mtls)
{
	// The provided file name is given as filename = "obj/FILE/file.obj"
	// As a result when we put the material destination into *objs
	// It will be missing "obj/FILE/"
	// To fix this we take everything in filename before the final '/'
	// and add this to before the .mtl file provided within the obj file.
	std::string fileNameAsString = filename;
	size_t lastSlashPos = fileNameAsString.find_last_of('/');
	std::string filePath = fileNameAsString.substr(0, lastSlashPos + 1);

	// Open the file
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("Material File opening failed\n");
		return 0;
	}

	std::optional<Material> currentMaterial;

	while (1) {
		char lineHeader[256];

		int endOfFile = fscanf(file, "%s", lineHeader);
		if (endOfFile == EOF) {
			break;
		}

		// When we are given a new material, we need to make a new material
		// Object and set the material name
		if (strcmp(lineHeader, "newmtl") == 0) {
			currentMaterial = Material();
			fscanf(file, "%s\n", currentMaterial->mtl_name);
		}

		// When given a file name for the material we need to set the current
		// material file name and then add the material to the vector *mtls
		if (strcmp(lineHeader, "map_Kd") == 0) {
			// We have the same problem with the file path as previously
			// when setting the fileName we need the full path so we 
			// concatenate the two together and then push the material to the vector
			char mtlFileName[256];
			char mtlFilePath[256];

			// as the fil_name is a char * we need to convert from a std::string
			strcpy(mtlFilePath, filePath.c_str());

			// Read the filename from the mtl file
			// and concatenate to the full file path
			fscanf(file, "%s\n", mtlFileName);
			strcat(mtlFilePath, mtlFileName);

			// Copy the filePath into the fil_name
			strcpy(currentMaterial->fil_name, mtlFilePath);

			mtls->push_back(currentMaterial.value());
		}
	}

	//success
	return 1;
}


int obj_parse(const char* filename, vector<Object>* objs)
 {
	// Stores the values parsed before reaching the first "usemtl"
	// So we can index them later
	vector<Material> materials;
	vector<vec3> vecs;
	vector<vec2> uvs;
	vector<vec3> normals;

	// The provided file name is given as filename = "obj/FILE/file.obj"
	// As a result when we put the material destination into *objs
	// It will be missing "obj/FILE/"
	// To fix this we take everything in filename before the final '/'
	// and add this to before the .mtl file provided within the obj file.
	std::string fileNameAsString = filename;
	size_t lastSlashPos = fileNameAsString.find_last_of('/');
	std::string filePath = fileNameAsString.substr(0, lastSlashPos + 1);

	// For each "usemtl" we need to build a new object
	// When we encounter a new "usemtl" we need to store the current object
	// and begin building a new one
	std::optional<Object> currentObject;

	// Open the file and error out if reading fails
	FILE* file = fopen(filename, "r");
	if (file == NULL) {
		printf("File opening failed\n");
		return 0;
	}

	while (1) {

		// The keyword at the start of each line
		char lineHeader[256];

		// Check to see if the file has ended
		// If it has push the last object onto the object vector
		int endOfFile = fscanf(file, "%s", lineHeader);
		if (endOfFile == EOF) {
			// put last object on vector *objs
			if (currentObject) {
				objs->push_back(currentObject.value());
			}
			break;
		}

		// Checks for a vertex position "v" and stores it on the vertex vector
		// for use later
		if (strcmp(lineHeader, "v") == 0) {
			vec3 vertex;
			fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
			vecs.push_back(vertex);
		}

		// Checks for a vertex texture "vt" and stores it in the "uv" vector
		// for use later
		else if (strcmp(lineHeader, "vt") == 0) {
			vec2 uv;
			fscanf(file, "%f %f\n", &uv.x, &uv.y);
			uvs.push_back(uv);
		}

		// Checks for a vertex normal "vn"
		else if (strcmp(lineHeader, "vn") == 0) {
			vec3 normal;
			fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
			normals.push_back(normal);
		}

		// Checks for the name of an associated MTL file
		// Takes the leading file path provided i.e. "obj/FILE/"
		// and pre-pends it to the fileName provided within the .obj file
		// Then calls mtl_parse with the full filename and a reference to
		// the materials vector declared above for later use
		else if (strcmp(lineHeader, "mtllib") == 0) {
			char mtlFileName[256];
			char mtlFilePath[256];
			strcpy(mtlFilePath, filePath.c_str());
			fscanf(file, "%s", mtlFileName);
			mtl_parse(strcat(mtlFilePath, mtlFileName), &materials);
		}


		// checks for a new material to be used for ab object "usemtl"
		// We do an initial check to see if there is already an object being created
		// (we use a flag for this as *optional* is not available in this version of C++)
		// If there is an object is progress we push that to the objects vector and begin a new object
		else if (strcmp(lineHeader, "usemtl") == 0) {
			if (currentObject) {
				objs->push_back(currentObject.value());
				currentObject = Object();
			}
			else {
				currentObject = Object();
			}


			// We then read in the materialName and check for the name within our *materials* vector
			// to set our objects material to the material with the same name
			char materialName[128];
			fscanf(file, "%s\n", materialName);

			for (Material material : materials) {
				if (strcmp(material.mtl_name, materialName) == 0) {
					currentObject->mtl = material;
				}
			}
		}

		// Check for "f" to start building a triangle
		else if (strcmp(lineHeader, "f") == 0) {

			// The values after f provide the index of the provided vertecies and uv values
			unsigned int vertexIndex[3], uvIndex[3], normalsIndex[3];

			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalsIndex[0], &vertexIndex[1], &uvIndex[1], &normalsIndex[1], &vertexIndex[2], &uvIndex[2], &normalsIndex[2]);

			// check to see if we have recieved the correct amount of values
			if (matches != 9) {
				printf("Something has been misread when reading *f*\n");
				return 0;
			}


			// Each triangle is constructed of 3 verticies
			// We need to take the index of the vertex and uv and subtract 1
			// As the indicies provided are indexed from 1 whereas our vector of verticies are indexed from 0
			// We then do this again for the texture coordinates and repeat 3 times for each vertex in the triangle
			triangle tri;
			for (int i = 0; i < 3; i++) {
				unsigned int indexOfVertex = vertexIndex[i];
				unsigned int indexOfUv = uvIndex[i];
				unsigned int indexOfNormal = normalsIndex[i];

				tri.verts[i].vc = vecs[indexOfVertex - 1];

				// The Y coordinates are flipped (0,0 is the bottom left in UV)
				// usually 0,0 as top left in i.e. screen space
				// so we have to flip the y to read the texture correctly
				float textureCoordX = uvs[indexOfUv - 1].x;
				float textureCoordY = uvs[indexOfUv - 1].y;

				tri.verts[i].tc = vec3(textureCoordX, textureCoordY, 0);

				//tri.verts[i].nc = normals[indexOfNormal - 1];
			}

			// We then push the vertex onto the current object we are building 
			currentObject->tris.push_back(tri);

		}

	}

	//success
	return 1;
}

