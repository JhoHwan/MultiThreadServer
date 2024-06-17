#pragma once
#include "VarCharPacket.h"
#include <string>

class ACK_CON : public VarCharPacket
{
public:
	ACK_CON();
	ACK_CON(const std::string&);

	~ACK_CON() {}
};

