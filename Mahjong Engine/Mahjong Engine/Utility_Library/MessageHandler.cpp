#include "MessageHandler.h"
#include "MessageQueue.h"
#include "Messages.h"
#include "../Dots/EntityHandler.h"

void MessageSystem::MessageHandler::CreateMessage(const std::string& aQueueName, const std::string& aMessage)
{
	myMessageQueues[aQueueName].QueueMessage(new MessageSystem::Message(MessageSystem::MessageType::Entity, aMessage));
}

void MessageSystem::MessageHandler::ProcessQueue(const std::string& aQueueName)
{
	myMessageQueues[aQueueName].ProcessMessages();
}

void MessageSystem::MessageHandler::ProcessMessage(Message* aMessage)
{
	// Switch case
	switch (aMessage->type)
	{
	case MessageSystem::MessageType::Entity:
		std::string& message = aMessage->message;

		if (message == "Request: Create Default Cube Entity")
		{
			// Grant permission (I'll later add strain checker and grant or queue the permission depending on that)
			// Create new message
			MessageSystem::Message* newMessage = new MessageSystem::Message(MessageSystem::MessageType::Entity, "CreateDefaultCube");
			DotsRendering::EntityHandler::GetInstance().ProcessMessages(newMessage);
		}
	}

	/*std::string& message = aMessage->message;

	if (message == "CreateDefaultCube")
	{
		CreateDefaultCube();
	}
	else if (message == "CreateDefaultSphere")
	{
		CreateDefaultSphere();
	}
	*/
}

MessageSystem::MessageHandler::MessageHandler()
{
}

MessageSystem::MessageHandler::~MessageHandler()
{
}
