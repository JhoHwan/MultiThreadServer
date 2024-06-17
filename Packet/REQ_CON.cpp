#include "REQ_CON.h"

REQ_CON::REQ_CON() : REQ_CON("")
{
}

REQ_CON::REQ_CON(const std::string& inId) : VarCharPacket(EPacketType::req_con, inId)
{
}
