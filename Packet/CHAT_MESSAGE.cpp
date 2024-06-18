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

    // 구분자 앞의 문자열을 추출
    if (std::getline(iss, segment, '|')) {
        id = segment;
    }

    // 구분자 뒤의 문자열을 추출
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