#pragma once
#include <memory>

class Packet;
struct PacketHeader;

/// <summary>
/// ���۸� ��Ŷ���� ��ȯ���ִ� Ŭ����
/// </summary>
class BufferReader
{
public:
	/// <summary>
	/// ���ۿ��� �ش��κи� �о� ��ȯ���ִ� �Լ�
	/// </summary>
	/// <param name="buffer">���� ����</param>
	/// <returns>���� ��Ŷ �ش�</returns>
	PacketHeader ReadHeader(const char* buffer);

	/// <summary>
	/// ���۸� ��Ŷ���� ��ȯ���ִ� �Լ� (�Լ� ��ü)
	/// </summary>
	/// <param name="source">���� ����</param>
	/// <param name="dest">��ȯ ��Ŷ</param>
	/// <returns>������ true ���н� false</returns>
	void operator() (const char* source, Packet* dest);
}; 


