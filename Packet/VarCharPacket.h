#pragma once
#include <string>

#include "Packet.h"

class VarCharPacket : public Packet
{
private:
	std::string data;

public:
	VarCharPacket(EPacketType inPacketType, std::string inData);

	inline const std::string& GetData() const { return data; }
	inline void SetData(std::string newData) { data = newData; }

	virtual void PacketToBuffer(char* outBuffer) const override;
	virtual void BufferToPacket(const char* inBuffer) override;

	bool IsValid(const char* buffer) const override;

	~VarCharPacket();
};

