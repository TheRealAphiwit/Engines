#pragma once
#include "Messages.h"
#include <vector>

namespace MessageSystem
{
	class MessageQueue
	{
	public:
		void QueueMessage(Message* aMessage);
		void ProcessMessages();

	private:
		void ProcessMessage(Message* aMessage);

		std::vector<Message*> messages;
	};
}