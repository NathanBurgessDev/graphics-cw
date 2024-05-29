#pragma once
#include "CompleteObject.h"
#include "castelijau.h"
class F22 : public CompleteObject
{
public:
	using CompleteObject::CompleteObject;
	std::vector<point> ctrl_points;
	std::vector<point> ctrl_points0;

	std::vector<float> pathVerticies;
	std::vector<float> ctrlVertices;

	std::vector<float> pathVerticies0;
	std::vector<float> ctrlVertices0;

	GLuint pathVAO, pathVBO;
	GLuint ctrlVAO, ctrlVBO;

	GLuint pathVAO0, pathVBO0;
	GLuint ctrlVAO0, ctrlVBO0;

	int num_curve_verts = 0;
	int num_curve_floats = 0;

	int num_ctrl_verts = 0;
	int num_ctrl_floats = 0;

	int num_curve_verts0 = 0;
	int num_curve_floats0 = 0;

	int num_ctrl_verts0 = 0;
	int num_ctrl_floats0 = 0;



	void setupCtrlPoints();
	void handleMovement(float currentTime, float deltaTime) override;
	void setupLineRender();
	void renderLines();
};

