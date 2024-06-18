#include "CHAT_MESSAGE.h"
#include <iostream>
#include <sstream>
#include <string>

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
    std::string segment;

    // ������ ���� ���ڿ��� ����
    if (std::getline(iss, segment, '|')) {
        id = segment;
    }

    // ������ ���� ���ڿ��� ����
    if (std::getline(iss, segment)) {
        message = segment;
    }

	SetData(message, id);
}

void CHAT_MESSAGE::SetData()
{
	std::ostringstream newData;

	newData << id << "|" << message;
	VarCharPacket::SetData(newData.str());
}