#include "CompleteObject.h"


CompleteObject::CompleteObject(const char* filename) {
	model = std::make_shared<glm::mat4>(1.f);
	obj_parse(filename, &objs, model);
	for (int i = 0; i < objs.size(); i++) {
		objs[i].setupTextureAndVAO();
	}
}

void CompleteObject::renderFullObject(unsigned int shaderProgram) {
	for (Object& obj : objs) {
		obj.renderObject(shaderProgram);		
	}
}

void CompleteObject::translate(float x, float y, float z) {
	*model = glm::translate((*model), glm::vec3(x, y, z));
}

void CompleteObject::scale(float x, float y, float z) {
	*model = glm::scale((*model), glm::vec3(x, y, z));
}