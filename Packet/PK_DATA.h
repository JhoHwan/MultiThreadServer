#pragma once
#include "Packet.h"

/// <summary>
/// PK_DATA: 고정길이 패킷(data : char[20})
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

	// 순수 가상함수 구현
	virtual void PacketToBuffer(char* outBuffer) const override;
	virtual void BufferToPacket(const char* inBuffer) override;

	virtual ~PK_DATA() override {}
};

