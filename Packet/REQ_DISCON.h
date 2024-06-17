#pragma once
#include "VarCharPacket.h"
#include <string>

class REQ_DISCON : public VarCharPacket
{
public:
	REQ_DISCON();
	REQ_DISCON(const std::string&);

	~REQ_DISCON() {}
};

