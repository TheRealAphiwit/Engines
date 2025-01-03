#include "MessageQueue.h"
#include "MessageQueue.h"

void MessageSystem::MessageQueue::QueueMessage(Message* aMessage)
{
	messages.push_back(aMessage);
}

void MessageSystem::MessageQueue::ProcessMessages()
{
	for (Message* message : messages)
	{
		ProcessMessage(message);
		delete message;
	}
}