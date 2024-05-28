#pragma once
#include "CompleteObject.h"
#include "castelijau.h"
class F22 : public CompleteObject
{
public:
	using CompleteObject::CompleteObject;
	std::vector<point> ctrl_points;
	std::vector<float> pathVerticies;
	std::vector<float> ctrlVertices;

	GLuint pathVAO, pathVBO;
	GLuint ctrlVAO, ctrlVBO;

	int num_curve_verts = 0;
	int num_curve_floats = 0;

	int num_ctrl_verts = 0;
	int num_ctrl_floats = 0;



	void setupCtrlPoints();
	void handleMovement(float currentTime, float deltaTime) override;
	void setupLineRender();
	void renderLines();
};

