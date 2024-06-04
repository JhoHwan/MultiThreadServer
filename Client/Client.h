#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS // 최신 VC++ 컴파일 시 경고 방지
#pragma comment(lib, "ws2_32")
#include <winsock2.h>

#include <iostream>
#include <map>
#include <functional>

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

public:
    Client();

    void Init();
    void Connect();
    void Run();

    ~Client();

private:
    void SendPacket(const Packet& packet);
    void RecivePacket();

    void ProcessPK_DATA(const char* buffer);
};

