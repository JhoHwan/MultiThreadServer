#include "VarCharPacket.h"

VarCharPacket::VarCharPacket(EPacketType inPacketType, std::string inData) :Packet(inPacketType, inData.length() + DefaultPacketSize + 1), data(inData)
{
}

void VarCharPacket::PacketToBuffer(char* outBuffer) const
{
	MakeDefaultBuffer(outBuffer);
	const char* packetData = data.c_str();
	
	memcpy(outBuffer + 3, packetData, data.length());
	memset(outBuffer + 3 + data.length(), '\0', 1);
}

void VarCharPacket::BufferToPacket(const char* inBuffer)
{
	unsigned short bufferSize = 0;
	memcpy(&bufferSize, inBuffer + 1, sizeof(unsigned short));
	
	char* bufferData = new char[bufferSize];
	memcpy(bufferData, inBuffer + 3, bufferSize);

	SetData(bufferData);
	SetSize(bufferSize);

	delete[] bufferData;
}

bool VarCharPacket::IsValid(const char* buffer) const
{
	unsigned short end;
	unsigned short size = 0;
	memcpy(&size, buffer + 1, sizeof(unsigned short));

	const char* temp = buffer + size - 2;
	memcpy(&end, temp, sizeof(unsigned short));

	if (end == endMarker) return true;
	else return false;
}

VarCharPacket::~VarCharPacket()
{
}
