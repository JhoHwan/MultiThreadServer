#pragma once
#include <cstring>
#include <iostream>
#include <memory>

#define DefaultPacketSize 5

/// <summary>
/// ��Ŷ ���� EnumType(char��)
/// </summary>
enum class EPacketType : char
{
	Invalid,
	PK_DATA,
	req_con,
	ack_con,
	ack_move,
	req_move,
	req_discon,
	ack_discon,
	chat_message,
	
};

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

/// <summary>
/// ��Ŷ �ش� ����ü(type, size)
/// </summary>
struct PacketHeader
{
	EPacketType type;
	unsigned short size;
	
	inline PacketHeader(EPacketType type, unsigned short size) : type(type), size(size) {}
};

/// <summary>
/// Packet �⺻ Ŭ���� (�߻� Ŭ����)
/// </summary>
class Packet
{
protected:
	PacketHeader header;
	const unsigned short endMarker;

protected:
	/// <summary>
	/// ��Ŷ�� �ش��� ���帶ũ�� ���ۿ� �Ű��ִ� �Լ�
	/// </summary>
	/// <param name="buffer">��Ŷ�� �ű� ����</param>
	virtual void MakeDefaultBuffer(char* buffer) const;
	inline void SetSize(unsigned short size) { header.size = size; }

public :
	Packet(PacketHeader header);
	Packet(EPacketType type, unsigned short size);

	//Getter �Լ�
	inline const PacketHeader& GetHeader() const { return header; }
	inline void SetPacketType(EPacketType newPacketType) { header.type = newPacketType; }

	inline EPacketType GetType() const { return header.type; }
	inline unsigned short GetSize() const { return header.size; }


	/// <summary>
	/// buffer�� �ش� ��Ŷ�� �´��� Ȯ���ϴ� �Լ�
	/// </summary>
	/// <param name="buffer">Ȯ���� ����</param>
	/// <returns>���ۿ� ��Ŷ���� ��ġ�ϸ� true, �ƴϸ� false</returns>
	virtual bool IsValid(const char* buffer) const;

	/// <summary>
	/// ��Ŷ�� ���ۿ� �Ű��ִ� �Լ�(���� �����Լ�)
	/// </summary>
	/// <param name="outBuffer">��Ŷ�� �ű� ����</param>
	virtual void PacketToBuffer(char* outBuffer) const = 0;

	/// <summary>
	/// ���ۿ� ���� �о� ��Ŷ�� �Ű��ִ� �Լ�(���� �����Լ�)
	/// </summary>
	/// <param name="inBuffer">�о�� ����</param>
	virtual void BufferToPacket(const char* inBuffer) = 0;

	virtual ~Packet();
};