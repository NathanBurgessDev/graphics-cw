#include "Tree.h"

Tree::Tree(GLuint shaderProgram, const char* filename) {
	this->shaderProgram = shaderProgram;
	model = std::make_shared<glm::mat4>(1.f);
	obj_parse(filename, &objs, model);
	for (int i = 0; i < objs.size(); i++) {
		if (i == 3 || i == 4)
			objs[i].hasInside = false;
		objs[i].setupTextureAndVAO();
		
	}
}