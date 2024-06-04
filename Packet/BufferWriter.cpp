#include "BufferWriter.h"
#include "Packet.h"

void BufferWriter::operator()(const Packet& source, char* dest)
{
	source.PacketToBuffer(dest);
}
