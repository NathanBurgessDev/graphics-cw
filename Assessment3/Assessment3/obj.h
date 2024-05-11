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

#include <string>

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
	vec3 vc;
	vec3 tc;
	//vec3 nc;

	vertex() {}
	vertex(vec3 vc_in, vec3 tc_in, vec3 nc_in)
	{
		vc = vc_in;
		tc = tc_in;
		//nc = nc_in;
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
	char mtl_name[256];
	char fil_name[256];

	Material() {}
	Material(char* n, char* f)
	{
		strcpy(mtl_name, n);
		strcpy(fil_name, f);
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
	glm::mat4 model = glm::mat4(1.f);


	Object() {}
	Object(Material m)
	{
		strcpy(mtl.fil_name, m.fil_name);
		strcpy(mtl.mtl_name, m.mtl_name);
	}
	void renderObject(unsigned int shaderProgram);
	void setupTextureAndVAO();
	~Object()
	{
	}
};

int mtl_parse(char* filename, vector<Material>* mtls);

int obj_parse(const char* filename, vector<Object>* objs);

