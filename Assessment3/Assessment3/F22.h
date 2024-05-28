#pragma once
#include "CompleteObject.h"
#include "castelijau.h"
class F22 : public CompleteObject
{
public:
	using CompleteObject::CompleteObject;
	std::vector<point> ctrl_points;
	void setupCtrlPoints();
	void handleMovement(float currentTime, float deltaTime) override;
};

