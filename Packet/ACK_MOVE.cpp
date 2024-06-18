#include "ACK_MOVE.h"
#include <sstream>

ACK_MOVE::ACK_MOVE() : ACK_MOVE("", Vector3())
{
}

ACK_MOVE::ACK_MOVE(const std::string& inId, const Vector3& pos) : VarCharPacket(EPacketType::ack_move, ""), id(inId), pos(pos)
{
	SetData();
}

void ACK_MOVE::SetData(const std::string& newId, const Vector3& newPos)
{
	id = newId;
	pos = newPos;
	SetData();
}

void ACK_MOVE::Deserialize(const char* inBuffer)
{
	VarCharPacket::Deserialize(inBuffer);
	std::string data = GetData();
	std::string posBuf = data.substr(0, 11);
	std::string newId = data.substr(13);

	Vector3 newPos;

	int temp = 0;
	memcpy(&temp, posBuf.c_str(), sizeof(int));
	newPos.x = temp;

	memcpy(&temp, posBuf.c_str() + sizeof(int), sizeof(int));
	newPos.y = temp;

	memcpy(&temp, posBuf.c_str() + sizeof(int) * 2, sizeof(int));
	newPos.z = temp;

	SetId(newId);
	SetPos(newPos);
}

void ACK_MOVE::SetData()
{
	std::ostringstream newData;

	char posBuf[sizeof(int) * 3] = { 0, };
	memcpy(posBuf, &(pos.x), sizeof(int));
	memcpy(posBuf + sizeof(int), &(pos.y), sizeof(int));
	memcpy(posBuf + sizeof(int) * 2, &(pos.z), sizeof(int));
	std::string posBufStr(posBuf, 12);

	newData << posBufStr << " " << id;
	VarCharPacket::SetData(newData.str());
}
