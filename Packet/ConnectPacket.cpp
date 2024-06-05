#include "ConnectPacket.h"

ConnectPacket::ConnectPacket()
{
}

ConnectPacket::ConnectPacket(const std::string& id) : Packet(EPacketType::ack_con, id.size() + DefaultPacketSize), id(id)
{

}

void ConnectPacket::PacketToBuffer(char* outBuffer) const
{
	MakeDefaultBuffer(outBuffer);
	const char* idBuffer = id.c_str();

	memcpy(outBuffer + 3, idBuffer, id.size());
}

void ConnectPacket::BufferToPacket(const char* inBuffer)
{
	unsigned short size;
	memcpy(&size, inBuffer + 1, 2);

	char* id = new char[size];
	memcpy(id, inBuffer + 3, size);
	SetId(id);

	delete[] id;
}
