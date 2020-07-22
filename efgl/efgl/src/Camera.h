#pragma once
#include "efpch.h"

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>

namespace efgl {

	enum CameraMovement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};

	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float SPEED = 2.5F;
	const float SENSITIVITY = 0.1f;
	const float ZOOM = 45.0f;

	class Camera
	{
	public:
		glm::vec3 Position;
		glm::vec3 Front;
		glm::vec3 Up;
		glm::vec3 Right;
		glm::vec3 WorldUp;

		float Yaw;
		float Pitch;

		float MovementSpeed;
		float MouseSensitivity;
		float Zoom;

		Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH)
			: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY),
			Zoom(ZOOM)
		{
			Position = position;
			WorldUp = up;
			Yaw = yaw;
			Pitch = pitch;
			updateCameraVectors();
		}

		Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
			: Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
		{
			Position = glm::vec3(posX, posY, posZ);
			WorldUp = glm::vec3(upX, upY, upZ);
			Yaw = yaw;
			Pitch = pitch;
			updateCameraVectors();
		}

		glm::mat4 GetViewMatrix() {
			return glm::lookAt(Position, Position + Front, Up);
		}

		void ProcessKeyboard(CameraMovement direction, float deltaTime) {
			float velocity = MovementSpeed * deltaTime;
			switch (direction) {
			case FORWARD:
				Position += Front * velocity;
				break;
			case BACKWARD:
				Position -= Front * velocity;
				break;
			case LEFT:
				Position -= Right * velocity;
				break;
			case RIGHT:
				Position += Right * velocity;
				break;
			}
		}

		void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true) {
			xOffset *= MouseSensitivity;
			yOffset *= MouseSensitivity;

			Yaw += xOffset;
			Pitch += yOffset;

			if (constrainPitch) {
				Pitch = (Pitch > 89.0f) ? 89.0f : Pitch;
				Pitch = (Pitch < -89.0f) ? -89.0f : Pitch;
			}

			updateCameraVectors();
		}

		void ProcessMouseScroll(float yOffset) {
			Zoom -= (float)yOffset;
			Zoom = (Zoom < 1.0f) ? 1.0f : Zoom;
			Zoom = (Zoom > 45.0f) ? 45.0f : Zoom;
		}
	private:

		void updateCameraVectors()
		{
			glm::vec3 front;
			front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			front.y = sin(glm::radians(Pitch));
			front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
			Front = glm::normalize(front);

			Right = glm::normalize(glm::cross(Front, WorldUp));
			Up = glm::normalize(glm::cross(Right, Front));
		}
	};
}