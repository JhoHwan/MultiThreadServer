#include "ACK_DISCON.h"

ACK_DISCON::ACK_DISCON() : ACK_DISCON("")
{
}

ACK_DISCON::ACK_DISCON(const std::string& inId) : VarCharPacket(EPacketType::ack_discon, inId)
{
}
