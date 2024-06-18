#include "REQ_MOVE.h"

REQ_MOVE::REQ_MOVE() : REQ_MOVE(Vector3())
{
}

REQ_MOVE::REQ_MOVE(const Vector3& pos) : Packet(EPacketType::req_move, DefaultPacketSize + 12), pos(pos)
{
}

void REQ_MOVE::Serialize(char* outBuffer) const
{
	MakeDefaultBuffer(outBuffer);

	memcpy(outBuffer + 3, &(pos.x), sizeof(int));
	memcpy(outBuffer + 3 + sizeof(int), &(pos.y), sizeof(int));
	memcpy(outBuffer + 3 + sizeof(int) * 2, &(pos.z), sizeof(int));
}

void REQ_MOVE::Deserialize(const char* inBuffer)
{
	int temp = 0;

	memcpy(&temp, inBuffer + 3, sizeof(int));
	pos.x = temp;

	memcpy(&temp, inBuffer + 3 + sizeof(int), sizeof(int));
	pos.y = temp;

	memcpy(&temp, inBuffer + 3 + sizeof(int) * 2, sizeof(int));
	pos.z = temp;
}
