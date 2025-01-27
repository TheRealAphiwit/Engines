#include "MessageQueue.h"
#include "MessageQueue.h"
#include "MessageQueue.h"
#include "MemoryChecker.h"
#include "../Dots/EntityHandler.h"

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

	std::string& message = aMessage->message;

	if (message == "Request: Thread Create Default Cube Entity")
	{
		// Grant permission (I'll later add strain checker and grant or queue the permission depending on that)
		// Create new message
		MessageSystem::Message* newMessage = new MessageSystem::Message(MessageSystem::MessageType::Entity, "ThreadCreateDefaultCube");
		DotsRendering::EntityHandler::GetInstance().ProcessMessages(newMessage);

		// Memory
		MemoryChecker memoryChecker;
		memoryChecker.PrintMemoryStatus();
	}

	// Old switch case
	//switch (aMessage->type)
	//{
	//case MessageType::Entity:
	//	// Call entity handler
	//	DotsRendering::EntityHandler::GetInstance().ProcessMessages(aMessage);
	//	break;
	//}
}
