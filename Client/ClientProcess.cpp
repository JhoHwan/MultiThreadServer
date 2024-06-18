#include "Client.h"

void Client::ProcessPK_DATA(const char* buffer)
{
    PK_DATA packet;
    reader(buffer, &packet);
    display->DrawMessage("[받은 데이터] %s", packet.GetData());
}


void Client::ProcessACK_CON(const char* buffer)
{
    ACK_CON packet;
    reader(buffer, &packet);
    std::string id = packet.GetData();
    display->DrawMessage("[%s]님이 입장하였습니다.", id.c_str());
}

void Client::ProcessACK_DISCON(const char* buffer)
{
    ACK_DISCON packet;
    reader(buffer, &packet);
    std::string id = packet.GetData();
    display->DrawMessage("[%s]님이 퇴장하였습니다.\n", id.c_str());
}

void Client::ProcessACK_MOVE(const char* buffer)
{
    ACK_MOVE packet;
    reader(buffer, &packet);
    std::string id = packet.GetId();
    Vector3 pos = packet.GetPos();

    display->DrawMessage("[%s] move to %d %d %d", id.c_str(), pos.x, pos.y, pos.z);
}

void Client::ProcessCHAT_MESSAGE(const char* buffer)
{
    CHAT_MESSAGE packet;
    reader(buffer, &packet);
    std::string id = packet.GetId();
    std::string message = packet.GetMessageString();
    
    display->DrawMessage("[%s] : %s", id.c_str(), message.c_str());
}