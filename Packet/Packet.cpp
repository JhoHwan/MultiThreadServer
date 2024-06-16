#include "Packet.h"

Packet::Packet(PacketHeader header) : header(header), endMarker(0xffff)
{
}

Packet::Packet(EPacketType type, unsigned short size) : header(PacketHeader(type, size)), endMarker(0xffff)
{
}

void Packet::MakeDefaultBuffer(char* buffer) const
{
	//버퍼에 앞 3바이트를 해더로 뒤 2바이트를 앤드마크로 씀
	memcpy(buffer, &(header.type), sizeof(header.type));
	memcpy(buffer + 1, &(header.size), sizeof(header.size));
	memcpy(buffer + header.size - 2, &endMarker, sizeof(endMarker));
}

bool Packet::IsValid(const char* buffer) const
{
	// 버퍼의 마지막 2바이트 읽어 앤드마크인지 확인
	unsigned short end;
	unsigned short size = GetSize();
	const char* temp = buffer + size - 2;	
	memcpy(&end, temp, sizeof(unsigned short));

	if (end == endMarker) return true;
	else return false;
}

Packet::~Packet()
{
}