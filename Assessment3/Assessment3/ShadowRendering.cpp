#include "ShadowRendering.h"


ShadowRendering::ShadowRendering(int width, int height) {
	this->width = width;
	this->height = height;
}
void ShadowRendering::generateDepthMap(unsigned int shadowShaderProgram,std::vector<CompleteObject>& objs,glm::mat4 projectedLightSpaceMatrix) {
	glViewport(0, 0, sh_map_width, sh_map_height);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMap.FBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	glUseProgram(shadowShaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(shadowShaderProgram, "projectedLightSpaceMatrix"), 1, GL_FALSE, glm::value_ptr(projectedLightSpaceMatrix));
	//drawFloorAndCubes(shadowShaderProgram);
	glCullFace(GL_FRONT);
	for (CompleteObject& obj : objs) {
		obj.renderFullObjectWithShader(shadowShaderProgram);
	}
	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}



