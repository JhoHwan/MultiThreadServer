#pragma once

class Packet;


/// <summary>
/// 패킷을 버퍼로 변환해주는 클래스
/// </summary>
class BufferWriter
{
public:
	/// <summary>
	/// 패킷을 버퍼로 변환해주는 함수 객체
	/// </summary>
	/// <param name="source">변환할 패킷</param>
	/// <param name="dest">버퍼</param>
	void operator() (const Packet& source, char* dest);
};

