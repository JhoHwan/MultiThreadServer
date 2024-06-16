#include "Client.h"


Client::Client()
{
}

void Client::Init()
{
    // 윈속 초기화
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        throw "WSAStartup Error";

    // socket()
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == INVALID_SOCKET) throw "socket() Error";
    
    packetFuncMap.emplace(EPacketType::PK_DATA, std::bind(&Client::ProcessPK_DATA, this, std::placeholders::_1));
    packetFuncMap.emplace(EPacketType::ack_con, std::bind(&Client::ProcessAck_con, this, std::placeholders::_1));
    packetFuncMap.emplace(EPacketType::ack_discon, std::bind(&Client::ProcessAck_discon, this, std::placeholders::_1));
}

void Client::Connect()
{
    // connect()
    SOCKADDR_IN serveraddr;
    ZeroMemory(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
    serveraddr.sin_port = htons(SERVERPORT);

    int retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
    if (retval == SOCKET_ERROR)
    {
        throw "Connect() Error!";
    }
   

    VarCharPacket req_con(EPacketType::req_con, GetUserId().c_str());
    SendPacket(req_con);

    packetReciveThread = std::thread(&Client::PacketReciver, this);
    packetReciveThread.detach();

    std::cout << "Server connect success!" << std::endl;
}

void Client::Run()
{
    int in = 0;
    while (1)
    {
        std::cin >> in;
        
        if (in == -1)
            break;
    }

}

void Client::SendPacket(const Packet& packet)
{
    char buffer[50];
    writer(packet, buffer);
    int packetSize = packet.GetSize();
    // 데이터 보내기
    int retval = send(sock, buffer, packetSize, 0);
    if (retval == SOCKET_ERROR) {
        std::cout << "SendPacket Failed" << std::endl;;
        return;
    }
    printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);
}

void Client::PacketReciver()
{
    const int bufferSize = 50;
    char buffer[50];

    while (true)
    {
        int retval = recv(sock, buffer, bufferSize, 0);
        if (retval == SOCKET_ERROR || retval == 0)
        {
            std::cout << "SendPacket Failed" << std::endl;;
            return;
        }

        //printf("[TCP 클라이언트] %d바이트를 받았습니다.\n", retval);

        PacketHeader header = reader.ReadHeader(buffer);

        auto func = packetFuncMap[header.type];

        func(buffer);
    }
}


void Client::ProcessPK_DATA(const char* buffer)
{
    PK_DATA packet;
    reader(buffer, &packet);
    printf("[받은 데이터] %s\n", packet.GetData());
}


void Client::ProcessAck_con(const char* buffer)
{
    VarCharPacket packet(EPacketType::ack_con, "");
    reader(buffer, &packet);
    std::string id = packet.GetData();
    printf("[%s]님이 입장하였습니다.\n", id.c_str());
}

void Client::ProcessAck_discon(const char* buffer)
{
    VarCharPacket packet(EPacketType::ack_discon, "");
    reader(buffer, &packet);
    std::string id = packet.GetData();
    printf("[%s]님이 퇴장하였습니다.\n", id.c_str());
}

Client::~Client()
{
    VarCharPacket packet(EPacketType::req_discon, GetUserId());
    SendPacket(packet);

    // closesocket()
    closesocket(sock);
    packetReciveThread.~thread();
    // 윈속 종료
    WSACleanup();
}