#include "CHAT_MESSAGE.h"
#include <sstream>

CHAT_MESSAGE::CHAT_MESSAGE() : CHAT_MESSAGE("", "")
{

}

CHAT_MESSAGE::CHAT_MESSAGE(const std::string& id, const std::string& inMessage) : VarCharPacket(EPacketType::chat_message, ""), id(id), message(inMessage)
{
	SetData();
}

void CHAT_MESSAGE::Deserialize(const char* inBuffer)
{
	VarCharPacket::Deserialize(inBuffer);

	std::istringstream iss(GetData());
	std::string id, message;
	iss >> id >> message;

	SetData(message, id);
}

void CHAT_MESSAGE::SetData()
{
	std::ostringstream newData;

	newData << id << " " << message;
	VarCharPacket::SetData(newData.str());
}