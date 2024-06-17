#include "REQ_DISCON.h"

REQ_DISCON::REQ_DISCON() : REQ_DISCON("")
{

}

REQ_DISCON::REQ_DISCON(const std::string& inId) : VarCharPacket(EPacketType::req_discon, inId)
{

}
