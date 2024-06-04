#include "PK_DATA.h"

PK_DATA::PK_DATA() : Packet(EPacketType::PK_DATA, 25), data("")
{
}

PK_DATA::PK_DATA(const char* data) : PK_DATA()
{
	SetData(data);
}

void PK_DATA::SetData(const char* inData)
{
	// ���� �������� ũ�Ⱑ 20���� ũ�� ���� ���
	if (strlen(inData) > 20)
	{
		strcpy_s(data, "Data Overflow");
		return;
	}

	strcpy_s(data, inData);
}

void PK_DATA::PacketToBuffer(char* outBuffer) const
{
	Packet::MakeDefaultBuffer(outBuffer);
	memcpy(outBuffer + 3, data, 20);
}

void PK_DATA::BufferToPacket(const char* inBuffer)
{
	// ���� �������� ũ�Ⱑ 20���� ũ�� ���� ���
	char bufferData[20] = { 0, };
	if (strlen(inBuffer) > 20)
	{
		strcpy_s(bufferData, "Data Overflow");
	}
	else
	{
		memcpy(bufferData, inBuffer + 3, 20);
	}

	SetData(bufferData);
}
