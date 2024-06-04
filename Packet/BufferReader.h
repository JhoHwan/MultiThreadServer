#pragma once
#include <memory>

class Packet;
struct PacketHeader;

/// <summary>
/// 버퍼를 패킷으로 변환해주는 클래스
/// </summary>
class BufferReader
{
public:
	/// <summary>
	/// 버퍼에서 해더부분만 읽어 반환해주는 함수
	/// </summary>
	/// <param name="buffer">읽을 버퍼</param>
	/// <returns>읽은 패킷 해더</returns>
	PacketHeader ReadHeader(const char* buffer);

	/// <summary>
	/// 버퍼를 패킷으로 변환해주는 함수 (함수 객체)
	/// </summary>
	/// <param name="source">읽을 버퍼</param>
	/// <param name="dest">반환 패킷</param>
	/// <returns>성공시 true 실패시 false</returns>
	void operator() (const char* source, Packet* dest);
}; 


