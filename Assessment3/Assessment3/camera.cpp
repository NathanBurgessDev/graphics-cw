#include "camera.h"


SCamera::SCamera(glm::vec3 position,
	float yaw, float pitch)
{
	Front = glm::vec3(0.0f, 0.0f, -1.0f);
	Position = position;
	Up = glm::vec3(0.0f, 1.0f, 0.0f);
	WorldUp = Up;
	Right = glm::normalize(glm::cross(Front, WorldUp));

	Yaw = -90.f;
	Pitch = 0.f;
}


void SCamera::UpdateCamera()
{
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	Front = glm::normalize(front);

	Right = glm::normalize(glm::cross(Front, WorldUp));
	Up = glm::normalize(glm::cross(Right, Front));
}

glm::mat4 SCamera::GetViewMatrix()
{
    return glm::lookAt(Position, Position + Front, Up);
}

void SCamera::ProcessMouseMovement(float xoffset, float yoffset)
{
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    
    
    if (Pitch > 89.0f)
        Pitch = 89.0f;
    if (Pitch < -89.0f)
        Pitch = -89.0f;
    

    // update Front, Right and Up Vectors using the updated Euler angles
	UpdateCamera();
}

void SCamera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD)
        Position += Front * velocity;
    if (direction == BACKWARD)
        Position -= Front * velocity;
    if (direction == LEFT)
        Position -= Right * velocity;
    if (direction == RIGHT)
        Position += Right * velocity;
    if (direction == SPEEDUP)
        MovementSpeed += 0.5f;
    if (direction == SPEEDDOWN)
        MovementSpeed -= 0.5f;
}
