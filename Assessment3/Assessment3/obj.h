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
#include <optional>
#include <memory>
#include <string>
#include <iostream>

#include "texture.h"

using namespace std;

class vec3
{
public:
	float x, y, z;

	vec3() {}
	vec3(float l, float m, float r)
	{
		x = l;
		y = m;
		z = r;
	}
	~vec3() {}
};

class vec2
{
public:
	float x, y;

	vec2() {}
	vec2(float l, float r)
	{
		x = l;
		y = r;
	}
	~vec2() {}
};

struct vertex
{
public:
	glm::vec3 vc;
	glm::vec2 tc;
	glm::vec3 nc;

	vertex() {}
	vertex(glm::vec3 vc_in, glm::vec2 tc_in, glm::vec3 nc_in)
	{
		vc = vc_in;
		tc = tc_in;
		nc = nc_in;
		
	}
	~vertex() {}
};
struct triangle
{
public:
	vertex verts[3];

	triangle() {}
	triangle(vertex v0, vertex v1, vertex v2)
	{
		verts[0] = v0;
		verts[1] = v1;
		verts[2] = v2;
	}
	~triangle() {}
};

class Material
{
public:
	std::string mtl_name;
	std::string fil_name;
	float specularHighlight = 128;
	glm::vec3 ambientColour = glm::vec3(0.5f,0.5f,0.5f), diffuseColour = glm::vec3(1.f, 1.f, 1.f), specularColour = glm::vec3(0.5f, 0.5f, 0.5f);

	Material() {}
	Material(char* n, char* f)
	{
		mtl_name = n;
		fil_name = f;
	}
	~Material()
	{
	}
};

class Object
{
public:
	unsigned int VAO;
	unsigned int VBO;
	vector<triangle> tris;
	Material mtl;
	GLuint texture;
	std::shared_ptr<glm::mat4> model;
	GLuint programType;
	bool hasInside = true;

	Object();
	Object(std::shared_ptr<glm::mat4> modelIn) {
		model = modelIn;
	}
	Object(Material m)
	{
		mtl.fil_name = m.fil_name;
		mtl.mtl_name = m.mtl_name;
	}
	void renderObject(unsigned int shaderProgram);
	void setupTextureAndVAO();
	void renderShadowObject(unsigned int shaderProgram);
	~Object()
	{
	}
};

int mtl_parse(char* filename, vector<Material>* mtls);

int obj_parse(const char* filename, vector<Object>* objs, std::shared_ptr<glm::mat4> model);

