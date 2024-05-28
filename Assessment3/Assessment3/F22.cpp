#include "F22.h"

void F22::setupCtrlPoints(){
	ctrl_points.push_back(point(0.f, 2.f, 0.f));
	ctrl_points.push_back(point(20.f, 2.f, 0.f));

	ctrl_points.push_back(point(30.f, 2.f, -10.f));
	ctrl_points.push_back(point(30.f, 2.f, -30.f));

	ctrl_points.push_back(point(20.f, 2.f, -40.f));
	ctrl_points.push_back(point(0.f, 2.f, -40.f));

	ctrl_points.push_back(point(-10.f, 2.f, -30.f));
	ctrl_points.push_back(point(-10.f, 2.f, -10.f));
	ctrl_points.push_back(point(0.f, 2.f, 0.f));
}

void F22::handleMovement(float currentTime, float deltaTime) {
	float modTime = std::fmod(currentTime/10.f, 1);
	//float modTimeFuture = std::fmod((currentTime + 5)/ 10.f, 1);
	point pointPos = evaluate(modTime, ctrl_points);
	point tangent = evaluateDerivative(modTime, ctrl_points);
	glm::vec3 tangentVec = glm::vec3(tangent.x, tangent.y, tangent.z);
	// CRoss product between y down
	tangentVec = glm::cross(tangentVec, glm::vec3(0, 1, 0));
	//float magnitude= glm::length(glm::vec3(tangent.x, tangent.y, tangent.z));
	glm::vec3 nTangent = glm::normalize(tangentVec);
	//point futurePointPos = evaluate(modTimeFuture, ctrl_points);
 	this->pos = glm::vec3(pointPos.x, pointPos.y, pointPos.z);
	this->directionVector = nTangent;

	std::cout << "Position" << std::endl;
	std::cout << this->pos.x << std::endl;
	std::cout << this->pos.y << std::endl;
	std::cout << this->pos.z << std::endl;

	std::cout << "Direction" << std::endl;

	std::cout << this->directionVector.value().x << std::endl;
	std::cout << this->directionVector.value().y << std::endl;
	std::cout << this->directionVector.value().z << std::endl;
}

void F22::setupLineRender() {
	std::vector<point> curve = EvaluateBezierCurve(ctrl_points, 20000);

	pathVerticies =	MakeFloatsFromVector(curve, num_curve_verts, num_curve_floats, 0.f, 0.f, 0.f);
	ctrlVertices = MakeFloatsFromVector(ctrl_points, num_ctrl_verts, num_ctrl_floats, 1.f, 0.f, 0.f);

	glGenVertexArrays(1, &pathVAO);
	glGenBuffers(1, &pathVBO);

	glBindVertexArray(pathVAO);
	glBindBuffer(GL_ARRAY_BUFFER, pathVBO);


	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_curve_floats, &pathVerticies[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glGenVertexArrays(1, &ctrlVAO);
	glGenBuffers(1, &ctrlVBO);

	glBindVertexArray(ctrlVAO);
	glBindBuffer(GL_ARRAY_BUFFER, ctrlVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_ctrl_floats, &ctrlVertices[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glPointSize(50);

	glBindVertexArray(0);

}


void F22::renderLines() {
	glBindVertexArray(pathVAO);
	glDrawArrays(GL_LINE_STRIP, 0, num_curve_verts);

	glBindVertexArray(ctrlVAO);
	glDrawArrays(GL_POINTS, 0, num_ctrl_verts);

	glBindVertexArray(0);
}