#include "CompleteObject.h"


CompleteObject::CompleteObject(const char* filename) {
	obj_parse(filename, &objs);
	for (int i = 0; i < objs.size(); i++) {
		objs[i].setupTextureAndVAO();
	}
}

void CompleteObject::renderFullObject(unsigned int shaderProgram) {
	for (Object obj : objs) {
		obj.renderObject(shaderProgram);
	}
}