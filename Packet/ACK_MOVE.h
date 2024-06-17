#pragma once
#include "VarCharPacket.h"

class ACK_MOVE : public VarCharPacket
{
private:
	std::string id;
	Vector3 pos;

	virtual inline void SetData(const std::string& newData) override
	{
		VarCharPacket::SetData(newData);
	}
	void SetData();

public:
	ACK_MOVE();
	ACK_MOVE(const std::string& inId, const Vector3& pos);

	void SetData(const std::string& newId, const Vector3& newPos);
	inline void SetId(const std::string& newId)
	{
		id = newId;
		SetData();
	}
	inline void SetPos(const Vector3& newPos)
	{
		pos = newPos;
		SetData();
	}

	inline const std::string& GetId() { return id; }
	inline const Vector3& GetPos() { return pos; }

	virtual void BufferToPacket(const char* inBuffer) override;

};

