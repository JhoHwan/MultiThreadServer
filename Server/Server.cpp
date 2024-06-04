#include "Server.h"
#include <stdio.h>
#include <Windows.h>
#include "Packet_Include.h"

Server::Server()
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

        // 접속한 클라이언트 정보 출력
        Log(ELogTypes::Log, "[클라이언트 접속] IP 주소=%s, 포트 번호=%d\n",
            inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

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

void Server::ProcessClient(const SOCKET& clientSocket)
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

    // closesocket()
    closesocket(clientSocket);
    printf("\n[Server Log] : [클라이언트 종료] IP 주소=%s, 포트 번호=%d\n",
        inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

    return ;
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
    BufferReader reader;
    PK_DATA recvPacket;

    SOCKADDR_IN clientaddr;
    int addrlen = sizeof(SOCKADDR_IN);
    getpeername(clientSock, (SOCKADDR*)&clientaddr, &addrlen);

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

    // 받은 데이터 출력
    printf("[TCP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
        ntohs(clientaddr.sin_port), recvPacket.GetData());

    int packetSize = recvPacket.GetSize();

    // 데이터 보내기
    int retval = send(clientSock, buffer, packetSize, 0);
    if (retval == SOCKET_ERROR) {
        Log(ELogTypes::Error, "send() Failed!");
    }

    return;
}

