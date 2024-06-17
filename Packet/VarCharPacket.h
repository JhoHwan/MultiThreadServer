#pragma once
#include <string>

#include "Packet.h"

class VarCharPacket : public Packet
{
private:
	std::string data;

public:
	VarCharPacket(EPacketType inPacketType, std::string inData);

	virtual inline const std::string& GetData() const { return data; }
	virtual inline void SetData(const std::string& newData) 
	{
		data = newData; 
		SetSize(DefaultPacketSize + data.length() + 1);
	}

	virtual void PacketToBuffer(char* outBuffer) const override;
	virtual void BufferToPacket(const char* inBuffer) override;

	bool IsValid(const char* buffer) const override;

	~VarCharPacket();
};

