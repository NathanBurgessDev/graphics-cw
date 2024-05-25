#include "CompleteObject.h"

CompleteObject::CompleteObject(){
}

CompleteObject::CompleteObject(GLuint shaderProgram, const char* filename) {
	this->shaderProgram = shaderProgram;
	model = std::make_shared<glm::mat4>(1.f);
	obj_parse(filename, &objs, model);
	for (int i = 0; i < objs.size(); i++) {
		objs[i].setupTextureAndVAO();
	}
}

CompleteObject::CompleteObject(GLuint shaderProgram) {
	this->shaderProgram = shaderProgram;
};

void CompleteObject::renderFullObject() {
	glUseProgram(shaderProgram);
 	for (Object& obj : objs) {
		obj.renderObject(shaderProgram);		
	}
}

void CompleteObject::renderFullObjectWithShader(GLuint newShaderProgram){
	for (Object& obj : objs) {
		obj.renderShadowObject(newShaderProgram);
	}
}

void CompleteObject::translate(float x, float y, float z) {
	*model = glm::translate((*model), glm::vec3(x, y, z));
}

void CompleteObject::scale(float x, float y, float z) {
	*model = glm::scale((*model), glm::vec3(x, y, z));
}