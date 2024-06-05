#pragma once

#include <string>
#include "Packet.h"

class ConnectPacket : public Packet
{
private:
	std::string id;

public:
	ConnectPacket();
	ConnectPacket(const std::string& id);

	inline const std::string& GetId() const { return id; }
	inline void SetId(const char* id) { this->id = id; SetSize(DefaultPacketSize + this->id.size()); }

	virtual void PacketToBuffer(char* outBuffer) const override;
	virtual void BufferToPacket(const char* inBuffer) override;

private:
};

