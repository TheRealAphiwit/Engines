#include "MessageQueue.h"
#include "MessageQueue.h"
#include "MessageQueue.h"
#include "EntityHandler.h"

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

void MessageSystem::MessageQueue::ProcessMessage(Message* aMessage)
{
	// Process and call different handlers based on message type

	switch (aMessage->type)
	{
	case MessageType::Entity:
		// Call entity handler
		EntityHandler::GetInstance().ProcessMessages(aMessage);
		break;
	}
}
