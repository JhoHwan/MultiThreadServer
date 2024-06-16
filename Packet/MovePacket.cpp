#include "MovePacket.h"

MovePacket::MovePacket(EPacketType type, Vector3 pos) : Packet(type, DefaultPacketSize + 12), pos(pos)
{
}

void MovePacket::PacketToBuffer(char* outBuffer) const
{
	MakeDefaultBuffer(outBuffer);

	memcpy(outBuffer + 3, &(pos.x), sizeof(int));
	memcpy(outBuffer + 3 + sizeof(int), &(pos.y), sizeof(int));
	memcpy(outBuffer + 3 + sizeof(int) * 2, &(pos.z), sizeof(int));
}

void MovePacket::BufferToPacket(const char* inBuffer)
{
	int temp = 0;

	memcpy(&temp, inBuffer + 3, sizeof(int));
	pos.x = temp;

	memcpy(&temp, inBuffer + 3 + sizeof(int), sizeof(int));
	pos.y = temp;

	memcpy(&temp, inBuffer + 3 + sizeof(int) * 2, sizeof(int));
	pos.z = temp;
}
