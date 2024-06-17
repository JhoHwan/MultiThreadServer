#pragma once
#include "VarCharPacket.h"
#include <string>

class REQ_CON : public VarCharPacket
{
public:
	REQ_CON();
	REQ_CON(const std::string& inId);


	~REQ_CON() {}
};

