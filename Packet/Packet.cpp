#include "Packet.h"

Packet::Packet(PacketHeader header) : header(header), endMarker(0xffff)
{
}

Packet::Packet(EPacketType type, unsigned short size) : header(PacketHeader(type, size)), endMarker(0xffff)
{
}

void Packet::MakeDefaultBuffer(char* buffer) const
{
	//���ۿ� �� 3����Ʈ�� �ش��� �� 2����Ʈ�� �ص帶ũ�� ��
	memcpy(buffer, &(header.type), sizeof(header.type));
	memcpy(buffer + 1, &(header.size), sizeof(header.size));
	memcpy(buffer + header.size - 2, &endMarker, sizeof(endMarker));
}

bool Packet::IsValid(const char* buffer) const
{
	// ������ ������ 2����Ʈ �о� �ص帶ũ���� Ȯ��
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