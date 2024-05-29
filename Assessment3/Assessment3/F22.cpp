#include "F22.h"

void F22::setupCtrlPoints(){
	ctrl_points.push_back(point(0.f, 1.f, 0.f));
	ctrl_points.push_back(point(20.f, 1.f, 0.f));

	ctrl_points.push_back(point(20.f, 2.f, -20.f));
	ctrl_points.push_back(point(0.f, 4.f, -20.f));

	ctrl_points0.push_back(point(0.f, 4.f, -20.f));
	ctrl_points0.push_back(point(-20.f, 6.f, -20.f));

	ctrl_points0.push_back(point(-20.f, 1.f, 0.f));
	ctrl_points0.push_back(point(0.f, 1.f, 0.f));


	//ctrl_points.push_back(point(0.f, 2.f, 0.f));
	//ctrl_points.push_back(point(50.f, 15.f, 25.f));
	//ctrl_points.push_back(point(25.f, 25.f, -75.f));
	////ctrl_points.push_back(point(0.f, -2.f, 50.f));
	//ctrl_points.push_back(point(0.f, 2.f, 0.f));
}

void F22::handleMovement(float currentTime, float deltaTime) {
	float modTime = std::fmod(currentTime/10.f, 2.f);
	//float modTimeFuture = std::fmod((currentTime + 5)/ 10.f, 1);
	point pointPos;
	point tangent;
	if (modTime < 1) {
		pointPos = evaluate(modTime, ctrl_points);
		tangent = evaluateDerivative(modTime, ctrl_points);
	}
	else{
		pointPos = evaluate(modTime - 1, ctrl_points0);
		tangent = evaluateDerivative(modTime - 1, ctrl_points0);
	}
	glm::vec3 tangentVec = glm::vec3(tangent.x, tangent.y, tangent.z);
	// CRoss product between y down
	//tangentVec = glm::cross(tangentVec, glm::vec3(0, 1, 0));
	//float magnitude= glm::length(glm::vec3(tangent.x, tangent.y, tangent.z));
	glm::vec3 nTangent = glm::normalize(tangentVec);
	//point futurePointPos = evaluate(modTimeFuture, ctrl_points);
 	this->pos = glm::vec3(pointPos.x, pointPos.y, pointPos.z);
	this->directionVector = nTangent;

}

void F22::setupLineRender() {
	std::vector<point> curve = EvaluateBezierCurve(ctrl_points, 20000);
	std::vector<point> curve0 = EvaluateBezierCurve(ctrl_points0, 20000);

	pathVerticies =	MakeFloatsFromVector(curve, num_curve_verts, num_curve_floats, 1.f, 0.f, 0.f);
	ctrlVertices = MakeFloatsFromVector(ctrl_points, num_ctrl_verts, num_ctrl_floats, 1.f, 0.f, 0.f);

	pathVerticies0 = MakeFloatsFromVector(curve0, num_curve_verts0, num_curve_floats0, 1.f, 0.f, 1.f);
	ctrlVertices0 = MakeFloatsFromVector(ctrl_points0, num_ctrl_verts0, num_ctrl_floats0, 0.f, 0.f, 1.f);

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

	glGenVertexArrays(1, &pathVAO0);
	glGenBuffers(1, &pathVBO0);

	glBindVertexArray(pathVAO0);
	glBindBuffer(GL_ARRAY_BUFFER, pathVBO0);


	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_curve_floats0, &pathVerticies0[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (6 * sizeof(float)), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	glGenVertexArrays(1, &ctrlVAO0);
	glGenBuffers(1, &ctrlVBO0);

	glBindVertexArray(ctrlVAO0);
	glBindBuffer(GL_ARRAY_BUFFER, ctrlVBO0);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_ctrl_floats0, &ctrlVertices0[0], GL_STATIC_DRAW);
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

	glBindVertexArray(pathVAO0);
	glDrawArrays(GL_LINE_STRIP, 0, num_curve_verts0);

	glBindVertexArray(ctrlVAO0);
	glDrawArrays(GL_POINTS, 0, num_ctrl_verts0);

	glBindVertexArray(0);
}