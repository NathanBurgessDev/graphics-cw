#include "camera.h"


void InitCamera(SCamera& in)
{
	in.Front = glm::vec3(0.0f, 0.0f, -1.0f);
	in.Position = glm::vec3(0.0f, 0.0f, 0.0f);
	in.Up = glm::vec3(0.0f, 1.0f, 0.0f);
	in.WorldUp = in.Up;
	in.Right = glm::normalize(glm::cross(in.Front, in.WorldUp));

	in.Yaw = -90.f;
	in.Pitch = 0.f;
}


void MoveAndOrientCamera(SCamera& in, glm::vec3 target, float distance, float xoffset, float yoffset)
{
	in.Yaw -= xoffset * in.MovementSpeed;
	in.Pitch -= yoffset * in.MovementSpeed;

	if (in.Pitch > 89.0f)
		in.Pitch = 89.0f;
	if (in.Pitch < -89.9f)
		in.Pitch = -89.0f;

	float px = cos(glm::radians(in.Yaw)) * cos(glm::radians(in.Pitch));
	float py = sin(glm::radians(in.Pitch));
	float pz = sin(glm::radians(in.Yaw)) * cos(glm::radians(in.Pitch));

	in.Position = glm::vec3(px, py, pz) * in.cam_dist;

	in.Front = glm::normalize(target - in.Position);

	in.Right = glm::normalize(glm::cross(in.Front, in.WorldUp));

	in.Up = glm::normalize(glm::cross(in.Right, in.Front));
}