#include "BufferReader.h"
#include "Packet.h"


PacketHeader BufferReader::ReadHeader(const char* buffer)
{
	EPacketType type = EPacketType::Invalid;
	unsigned short size = 0;

	// ���ۿ��� �ش� �κ� ����
	memcpy(&type, buffer, sizeof(EPacketType));
	memcpy(&size, buffer + sizeof(EPacketType), sizeof(unsigned short));
	//

	PacketHeader header(type, size);

	return header;
}

void BufferReader::operator()(const char* source, Packet* dest)
{
	//��Ŷ �ĺ� ���� �� 
	/*if (!dest->IsValid(source)) 
		throw "Packet is Invalid";*/

	dest->Deserialize(source);
}
