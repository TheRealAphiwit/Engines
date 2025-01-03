#pragma once
#include "Messages.h"
#include <glm.hpp>

namespace MessageSystem
{
	class RaycastMessage : public Message
	{
		RaycastMessage() : Message(MessageType::Raycast, "Raycast") {}
		~RaycastMessage() {}
		glm::vec3 origin;
		glm::vec3 direction;
		float maxDistance;
	};
}