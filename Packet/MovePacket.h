#pragma once
#include "Packet.h"

struct Vector3
{
	int x;
	int y;
	int z;

	Vector3(int x = 0, int y = 0, int z = 0) : x(x), y(y), z(z) {}
	Vector3 operator+(const Vector3& other)
	{
		Vector3 out(this->x + other.x, this->y + other.y, this->z + other.z);
		return out;
	}
};

class MovePacket : public Packet
{
private:
	Vector3 pos;

public:
	MovePacket(EPacketType type, Vector3 Pos);

	inline const Vector3& GetData() const { return pos; }

	virtual void PacketToBuffer(char* outBuffer) const override;
	virtual void BufferToPacket(const char* inBuffer) override;

};