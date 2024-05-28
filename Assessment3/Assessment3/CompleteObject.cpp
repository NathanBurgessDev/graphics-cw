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

glm::mat4 CompleteObject::calcMovement() {
	glm::mat4 tempModel = *model;
	//tempModel = glm::translate(tempModel, pos);
	if (directionVector.has_value()) {

		up = glm::vec3(0.f, 1.f, 0.f);
		right = glm::normalize(glm::cross(up, directionVector.value()));
		up = glm::cross(directionVector.value(), right);

		glm::mat4 rotationMatrix = glm::mat4(1.0f);
		rotationMatrix[0] = glm::vec4(right, 0.0f);
		rotationMatrix[1] = glm::vec4(up, 0.0f);
		rotationMatrix[2] = glm::vec4(directionVector.value(), 0.0f);

		glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), pos);

		tempModel = translationMatrix * (rotationMatrix);
	}
	tempModel = glm::scale(tempModel, scale);

	return tempModel;
}

void CompleteObject::renderFullObject(GLuint shadowTexture) {
	glUseProgram(shaderProgram);
	glm::mat4 modelCopy = *model;
	*model = calcMovement();
	GLuint shadowMapPos = glGetUniformLocation(shaderProgram, "shadowMap");
	glUniform1i(shadowMapPos, 1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, shadowTexture);
 	for (Object& obj : objs) {
		obj.renderObject(shaderProgram);		
	}
	*model = modelCopy;
}

void CompleteObject::stackPosition(glm::vec3 translation) {
	this->pos += translation;
}

void CompleteObject::setStackPosition(glm::vec3 translation) {
	this->pos = translation;
}

void CompleteObject::renderFullObjectWithShader(GLuint newShaderProgram){
	glm::mat4 modelCopy = *model;
	*model = calcMovement();
	for (Object& obj : objs) {
		obj.renderShadowObject(newShaderProgram);
	}
	*model = modelCopy;
}

void CompleteObject::setPos(float x, float y, float z) {
	*model = glm::translate((*model), glm::vec3(x, y, z));
}

void CompleteObject::setScale(float x, float y, float z) {
	*model = glm::scale((*model), glm::vec3(x, y, z));
}

void CompleteObject::handleMovement(float currentTime, float deltaTime) {

}

