#pragma once
#include <cstring>
#include <iostream>
#include <memory>

#define DefaultPacketSize 5

/// <summary>
/// 패킷 종류 EnumType(char형)
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
/// 패킷 해더 구조체(type, size)
/// </summary>
struct PacketHeader
{
	EPacketType type;
	unsigned short size;
	
	inline PacketHeader(EPacketType type, unsigned short size) : type(type), size(size) {}
};

/// <summary>
/// Packet 기본 클래스 (추상 클래스)
/// </summary>
class Packet
{
protected:
	PacketHeader header;
	const unsigned short endMarker;

protected:
	/// <summary>
	/// 패킷의 해더와 엔드마크를 버퍼에 옮겨주는 함수
	/// </summary>
	/// <param name="buffer">패킷을 옮길 버퍼</param>
	virtual void MakeDefaultBuffer(char* buffer) const;
	inline virtual void SetSize(unsigned short size) { header.size = size; }

public :
	Packet(PacketHeader header);
	Packet(EPacketType type, unsigned short size);

	//Getter 함수
	inline const PacketHeader& GetHeader() const { return header; }
	inline void SetPacketType(EPacketType newPacketType) { header.type = newPacketType; }

	inline EPacketType GetType() const { return header.type; }
	inline unsigned short GetSize() const { return header.size; }


	/// <summary>
	/// buffer가 해당 패킷이 맞는지 확인하는 함수
	/// </summary>
	/// <param name="buffer">확인할 버퍼</param>
	/// <returns>버퍼와 패킷형이 일치하면 true, 아니면 false</returns>
	virtual bool IsValid(const char* buffer) const;

	/// <summary>
	/// 패킷을 버퍼에 옮겨주는 함수(순수 가상함수)
	/// </summary>
	/// <param name="outBuffer">패킷을 옮길 버퍼</param>
	virtual void Serialize(char* outBuffer) const = 0;

	/// <summary>
	/// 버퍼에 값을 읽어 패킷에 옮겨주는 함수(순수 가상함수)
	/// </summary>
	/// <param name="inBuffer">읽어올 버퍼</param>
	virtual void Deserialize(const char* inBuffer) = 0;

	virtual ~Packet();
};