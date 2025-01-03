#pragma once
#include <string>

namespace MessageSystem
{

	enum class MessageType
	{
		String,
		Mouse,
		FloatMessage,
		Raycast,
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