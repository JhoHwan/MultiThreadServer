#pragma once
#include "Packet.h"

class REQ_MOVE : public Packet
{
private:
	Vector3 pos;

public:
	REQ_MOVE();
	REQ_MOVE(const Vector3& Pos);

	inline const Vector3& GetPos() const { return pos; }

	virtual void PacketToBuffer(char* outBuffer) const override;
	virtual void BufferToPacket(const char* inBuffer) override;
};