#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>

#include <iostream>
#include <map>
#include <functional>
#include <thread>

#include "Packet_Include.h"

#define SERVERIP   "127.0.0.1"
#define SERVERPORT 9000
#define BUFSIZE    512

class Client
{
private:
    BufferReader reader;
    BufferWriter writer;

    SOCKET sock;

    std::map<EPacketType, std::function<void(const char*)>> packetFuncMap;

    bool run;

private:
    std::string userId;
public:
    inline void SetUserId(const std::string& newId) { userId = newId; }
    inline const std::string& GetUserId() const { return userId; }

public:
    Client();

    void Init();
    void Connect();
    void Run();

    ~Client();

private:
    void SendPacket(const Packet& packet);
    void PacketReciver();

    std::thread packetReciveThread;

    void ProcessPK_DATA(const char* buffer);
    void ProcessACK_CON(const char* buffer);
    void ProcessACK_DISCON(const char* buffer);
    void ProcessACK_MOVE(const char* buffer);
    void ProcessCHAT_MESSAGE(const char* buffer);

};

