#pragma once

class Packet;


/// <summary>
/// ��Ŷ�� ���۷� ��ȯ���ִ� Ŭ����
/// </summary>
class BufferWriter
{
public:
	/// <summary>
	/// ��Ŷ�� ���۷� ��ȯ���ִ� �Լ� ��ü
	/// </summary>
	/// <param name="source">��ȯ�� ��Ŷ</param>
	/// <param name="dest">����</param>
	void operator() (const Packet& source, char* dest);
};

