#pragma once
#include <string>

namespace MessageSystem
{

	enum class MessageType
	{
		Graphics,
		UI,
		Physics,
		Entity,
	};

	class Message
	{
	public:
		Message(MessageType Type, std::string msg);
		virtual ~Message();

		MessageType type;
		std::string message;
	};
}