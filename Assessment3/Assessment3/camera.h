#pragma once

#include <stdio.h>
#include <glm/glm.hpp>

struct SCamera
{
	enum Camera_Movement
	{
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;

	glm::vec3 WorldUp;

	float Yaw;
	float Pitch;
	float cam_dist = 2.f;

	const float MovementSpeed = .5f;
	float MouseSensitivity = 1.f;
};

void InitCamera(SCamera& in);
void MoveAndOrientCamera(SCamera& in, glm::vec3 target, float distance, float xoffset, float yoffset);