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
	point pointPos = evaluate(modTime, ctrl_points);
	this->pos = glm::vec3(pointPos.x, pointPos.y, pointPos.z);
}