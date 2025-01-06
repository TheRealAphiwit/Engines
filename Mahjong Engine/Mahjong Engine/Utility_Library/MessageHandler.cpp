#include "MessageHandler.h"

void MessageSystem::MessageHandler::CreateMessage(const std::string& aQueueName, const std::string& aMessage)
{
	myMessageQueues[aQueueName].QueueMessage(new MessageSystem::Message(MessageSystem::MessageType::Entity, aMessage));
}

void MessageSystem::MessageHandler::ProcessQueue(const std::string& aQueueName)
{
	myMessageQueues[aQueueName].ProcessMessages();
}

MessageSystem::MessageHandler::MessageHandler()
{
}

MessageSystem::MessageHandler::~MessageHandler()
{
}
