#pragma once
#include <unordered_map>
#include <string>
#include "../Dots/MessageQueue.h" //lmao

namespace MessageSystem
{
	class MessageHandler
	{
	public:
		#pragma region Singelton Setup
		// Singelton
		static MessageHandler& GetInstance()
		{
			static MessageHandler instance;
			return instance;
		}

		// Prevent copy and overwriting
		MessageHandler(const MessageHandler&) = delete;
		MessageHandler& operator=(const MessageHandler&) = delete;
		#pragma endregion
	


	private:
		MessageHandler();
		~MessageHandler();

		std::unordered_map<std::string, MessageQueue> myMessageQueues;
	};
}