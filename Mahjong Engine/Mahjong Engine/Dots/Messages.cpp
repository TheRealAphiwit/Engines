#include "Messages.h"

MessageSystem::Message::Message(MessageType Type, std::string msg) : type(Type), message(msg)
{
}

MessageSystem::Message::~Message()
{
}

// Example of message in use
//Message msg(MessageType::Info, "This is an informational message");
//std::cout << "Type: " << static_cast<int>(msg.type) << "\n";
//std::cout << "Message: " << msg.message << "\n";