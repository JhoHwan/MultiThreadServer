#include "BufferReader.h"
#include "Packet.h"


PacketHeader BufferReader::ReadHeader(const char* buffer)
{
	EPacketType type = EPacketType::Invalid;
	unsigned short size = 0;

	// 버퍼에서 해더 부분 복사
	memcpy(&type, buffer, sizeof(EPacketType));
	memcpy(&size, buffer + sizeof(EPacketType), sizeof(unsigned short));
	//

	PacketHeader header(type, size);

	return header;
}

void BufferReader::operator()(const char* source, Packet* dest)
{
	//패킷 식별 오류 시 
	/*if (!dest->IsValid(source)) 
		throw "Packet is Invalid";*/

	dest->Deserialize(source);
}
