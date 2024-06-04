#pragma once
#include "Packet.h"

/// <summary>
/// PK_DATA: �������� ��Ŷ(data : char[20})
/// </summary>
class PK_DATA : public Packet
{
private:
	char data[20];

public:
	PK_DATA();
	PK_DATA(const char* data);

	//Getter
	inline const char* GetData() const { return data; };
	//Setter
	void SetData(const char* inData);

	// ���� �����Լ� ����
	virtual void PacketToBuffer(char* outBuffer) const override;
	virtual void BufferToPacket(const char* inBuffer) override;

	virtual ~PK_DATA() override {}
};

