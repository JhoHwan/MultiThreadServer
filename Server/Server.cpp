#define _CRT_SECURE_NO_WARNINGS
#include "Server.h"
#include <iostream>
#include <Windows.h>
#include <algorithm>
#include <ctime>
#include "Packet_Include.h"

Server::Server() : currentConnectedClient(0)
{
    memset(&serverSocket, 0, sizeof(SOCKET));
}

void Server::Init()
{
    Log(ELogTypes::Log, "Server Init Start");

    int retval;

    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        throw "WSAStartup() Failed!";
    }
    // socket()
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET)
    {
        throw "socket() Failed!";
    }

    // bind()
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
    serveraddr.sin_port = htons(SERVERPORT);
    retval = bind(serverSocket, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR)
    {
        throw "bind() Failed!";
    }


    retval = listen(serverSocket, SOMAXCONN);
    if (retval == SOCKET_ERROR) 
    {
        throw "listen() Failed!";
    }

    Log(ELogTypes::Log, "Server Init Success");
    Log(ELogTypes::Debug, "Debug Mode On!\n");

    packetFuncMap.emplace(EPacketType::PK_DATA, std::bind(&Server::ProcessPK_DATA, this, std::placeholders::_1, std::placeholders::_2));
    packetFuncMap.emplace(EPacketType::req_con, std::bind(&Server::ProcessReq_con, this, std::placeholders::_1, std::placeholders::_2));
    packetFuncMap.emplace(EPacketType::req_discon, std::bind(&Server::ProcessReq_discon, this, std::placeholders::_1, std::placeholders::_2));
    packetFuncMap.emplace(EPacketType::req_move, std::bind(&Server::ProcessReq_move, this, std::placeholders::_1, std::placeholders::_2));
}

void Server::Log(ELogTypes inLogType, const char* inLog, ...) const
{
    if (DebugMode == false && inLogType == ELogTypes::Debug) return;

    switch (inLogType)
    {
    case ELogTypes::Log:
        printf("[Server Log] : ");
        break;
    case ELogTypes::Debug:
        printf("[Debug] : ");
        break;
    case ELogTypes::Error:
        printf("[Server Error] : ");
        break;
    case ELogTypes::Invaild:
    default:
        break;
    }

    va_list ap;
    va_start(ap, inLog);
    vfprintf(stderr, inLog, ap);
    va_end(ap);

    printf("\n");
}

void Server::Run()
{
    SOCKET client_sock;
    SOCKADDR_IN clientaddr;
    int addrlen;
    int retval = 0;
    char buf[BUFSIZE + 1] = { 0, };

    Log(ELogTypes::Log, "Server Start!\n");

    while (1) 
    {
        // accept()
        addrlen = sizeof(clientaddr);
        client_sock = accept(serverSocket, (SOCKADDR*)&clientaddr, &addrlen);
        if (client_sock == INVALID_SOCKET) {
            continue;
        }
        clientList.push_back(client_sock);
        Log(ELogTypes::Log, "현재 접속한 인원 수: %d", ++currentConnectedClient);

        CreateClientThread(client_sock);
    }
}

void Server::DeInit()
{
    // closesocket()
    closesocket(serverSocket);
    // 윈속 종료
    WSACleanup();
}

void Server::ProcessClient(const SOCKET clientSocket)
{
    char buf[BUFSIZE + 1];

    SOCKADDR_IN clientaddr;
    int addrlen = sizeof(SOCKADDR_IN);
    getpeername(clientSocket, (SOCKADDR*)&clientaddr, &addrlen);

    // 클라이언트와 데이터 통신
    while (1) {
        // 데이터 받기
        int retval = recv(clientSocket, buf, BUFSIZE, 0);

        if (retval == SOCKET_ERROR) {
            break;
        }
        else if (retval == 0)
            break;

        Log(ELogTypes::Debug, "%d 바이트 수신", retval);

        ProcessPacket(clientSocket, buf);
    }

    return ;
}

void Server::SendPacket(const SOCKET& clientSock, const Packet& packet)
{
    char buffer[100];
    BufferWriter writer;
    writer(packet, buffer);

    unsigned short packetSize = packet.GetSize();

    int retval = send(clientSock, buffer, packetSize, 0);
    if (retval == SOCKET_ERROR) {
        Log(ELogTypes::Error, "send() Failed!");
    }
}

void Server::CreateClientThread(const SOCKET& clientSock)
{
    std::thread t1(&Server::ProcessClient, this, clientSock);
    t1.detach();
}

void Server::ProcessPacket(const SOCKET& clientSock, const char* buffer)
{
    BufferReader reader;

    SOCKADDR_IN clientaddr;
    int addrlen = sizeof(SOCKADDR_IN);
    getpeername(clientSock, (SOCKADDR*)&clientaddr, &addrlen);

    PacketHeader header = reader.ReadHeader(buffer);

    auto func = packetFuncMap[header.type];
   
    func(clientSock, buffer);
}

void Server::ProcessPK_DATA(const SOCKET& clientSock, const char* buffer)
{
    PK_DATA recvPacket;

    BufferReader reader;
    BufferWriter writer;

    //패킷 변환 실패시
    try
    {
        reader(buffer, &recvPacket);
    }
    catch (const char* e)
    {
        Log(ELogTypes::Error, "%s", e);
        return;
    }

    Log(ELogTypes::Debug, "패킷 타입 : PK_DATA");

    return;
}

void Server::ProcessReq_con(const SOCKET& clientSock, const char* buffer)
{
    VarCharPacket recvPacket(EPacketType::req_con, "");

    BufferReader reader;

    try
    {
        reader(buffer, &recvPacket);
    }
    catch (const char* e)
    {
        Log(ELogTypes::Error, "%s", e);
        return;
    }

    Log(ELogTypes::Debug, "패킷 타입 : req_con");

    time_t timer = time(NULL);
    tm* curTime;
    curTime = localtime(&timer);

    const std::string& data = recvPacket.GetData();

    // 받은 데이터 출력
    Log(ELogTypes::Log, "%d년도 %d월 %d일 %d시 %d분 %d초, ID: %s connected!\n",
        curTime->tm_year + 1900, curTime->tm_mon + 1, curTime->tm_mday, curTime->tm_hour, curTime->tm_min, curTime->tm_sec, data.c_str());

    int packetSize = recvPacket.GetSize();
    VarCharPacket packet(EPacketType::ack_con, data.c_str());

    for (auto sock : clientList)
    {
        SendPacket(sock, packet);
    }

    return;
}

void Server::ProcessReq_discon(const SOCKET& clientSock, const char* buffer)
{
    VarCharPacket recvPacket(EPacketType::req_discon, "");

    BufferReader reader;

    try
    {
        reader(buffer, &recvPacket);
    }
    catch (const char* e)
    {
        Log(ELogTypes::Error, "%s", e);
        return;
    }

    Log(ELogTypes::Debug, "패킷 타입 : req_discon");

    time_t timer = time(NULL);
    tm* curTime;
    curTime = localtime(&timer);

    const std::string& data = recvPacket.GetData();

    // 받은 데이터 출력
    Log(ELogTypes::Log, "%d년도 %d월 %d일 %d시 %d분 %d초, ID: %s disconnected!",
        curTime->tm_year + 1900, curTime->tm_mon + 1, curTime->tm_mday, curTime->tm_hour, curTime->tm_min, curTime->tm_sec, data.c_str());

    int packetSize = recvPacket.GetSize();
    VarCharPacket packet(EPacketType::ack_discon, data.c_str());

    auto it = std::find(clientList.cbegin(), clientList.cend(), clientSock);
    if (it == clientList.cend())
        Log(ELogTypes::Error, "Error!");
    else
        clientList.erase(it);

    // closesocket()
    closesocket(clientSock);

    Log(ELogTypes::Log, "현재 접속한 인원 수: %d", --currentConnectedClient);

    for (auto sock : clientList)
    {
        SendPacket(sock, packet);
    }



    return;
}

void Server::ProcessReq_move(const SOCKET& clientSock, const char* buffer)
{
    Vector3 pos;
    MovePacket recvPacket(EPacketType::req_con, pos);

    BufferReader reader;


    try
    {
        reader(buffer, &recvPacket);
    }
    catch (const char* e)
    {
        Log(ELogTypes::Error, "%s", e);
        return;
    }

    Log(ELogTypes::Debug, "패킷 타입 : req_move");

    pos = recvPacket.GetData();


    int packetSize = recvPacket.GetSize();
}

