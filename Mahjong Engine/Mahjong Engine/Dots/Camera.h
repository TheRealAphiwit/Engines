#pragma once
#include <glm.hpp>
#include <glfw3.h>

namespace DotsRendering
{
	class Camera
	{
		enum class ProjectionType
		{
			Perspective,
			Orthographic,
			NULL_TYPE
		};

	public:
		// My new additions
		ProjectionType myProjectionType;

		glm::mat4 myProjection;
		glm::mat4 myView;

		Camera(const float& aWidth, const float& aHeight);

		void Move(const glm::vec3& aMove);
		void CameraUpdate();

		void SetPosition(const glm::vec3& aPosition);
		void SetRotation(const glm::vec3& aRotation);
		void SetDirection(const glm::vec3& aDirection);

		glm::vec3 GetCameraPosition() const;
		glm::vec3 GetCameraRotation() const;
		void GetProjectionType();

	private:
		glm::vec3 myPosition;
		glm::vec3 myDirection;

		glm::vec3 myUp;
		glm::vec3 myRight;
	};
}