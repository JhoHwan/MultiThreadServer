#include "ACK_CON.h"

ACK_CON::ACK_CON() : ACK_CON("")
{
}

ACK_CON::ACK_CON(const std::string& inId) : VarCharPacket(EPacketType::ack_con, inId)
{
}
