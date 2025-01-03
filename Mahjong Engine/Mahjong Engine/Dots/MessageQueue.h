#pragma once
#include "Messages.h"
#include <vector>

namespace MessageSystem
{
	class MessageQueue
	{
	public:
		void QueueMessage(Message* aMessage);

	private:
		void ProcessMessages();
		void ProcessMessage(Message* aMessage);

		std::vector<Message*> messages;
	};
}