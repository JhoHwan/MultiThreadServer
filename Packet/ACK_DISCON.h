#pragma once
#include "VarCharPacket.h"
#include <string>
class ACK_DISCON: public VarCharPacket
{
public:
	ACK_DISCON();
	ACK_DISCON(const std::string&);

	~ACK_DISCON() {}
};

