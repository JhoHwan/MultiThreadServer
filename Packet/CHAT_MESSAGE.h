#pragma once
#include "VarCharPacket.h"

class CHAT_MESSAGE : public VarCharPacket
{
private:
	std::string message;
	std::string id;

	virtual inline void SetData(const std::string& newData) override
	{
		VarCharPacket::SetData(newData);
	}
	void SetData();

public:
	CHAT_MESSAGE();
	CHAT_MESSAGE(const std::string& id , const std::string& inMessage);

	inline void SetId(const std::string& newId)
	{
		id = newId;
		SetData();
	}

	inline void SetMessage(const std::string& newMessage)
	{
		id = newMessage;
		SetData();
	}
	inline void SetData(const std::string& newMessage, const std::string& newId)
	{
		id = newId;
		message = newMessage;
		SetData();
	}

	inline const std::string& GetId() const { return id; }
	inline const std::string& GetMessageString() const { return message; }

	virtual void Deserialize(const char* inBuffer) override;
};

