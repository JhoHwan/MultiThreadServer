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
    if (retval == SOCKET_ERROR) throw "connect() Error";

    std::cout << "Server connect success!" << std::endl;
}

void Client::Run()
{
    char inputBuffer[100];

    // 서버와 데이터 통신
    while (1) {
        // 데이터 입력
        printf("\n[보낼 데이터] ");
        if (fgets(inputBuffer, 100, stdin) == NULL)
            break;

        // '\n' 문자 제거
        int len = static_cast<int>(strlen(inputBuffer));

        if (inputBuffer[len - 1] == '\n')
            inputBuffer[len - 1] = '\0';
        if (strlen(inputBuffer) == 0)
            break;

        PK_DATA packet(inputBuffer);
        SendPacket(packet);

        RecivePacket();
        // 데이터 받기
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

void Client::RecivePacket()
{
    const int bufferSize = 50;
    char buffer[50];

    int retval = recv(sock, buffer, bufferSize, 0);
    if (retval == SOCKET_ERROR || retval == 0)
    {
        std::cout << "SendPacket Failed" << std::endl;;
        return;
    }
    printf("[TCP 클라이언트] %d바이트를 받았습니다.\n", retval);
    PacketHeader header = reader.ReadHeader(buffer);

    auto func = packetFuncMap[header.type];

    func(buffer);
}

void Client::ProcessPK_DATA(const char* buffer)
{
    PK_DATA packet;
    reader(buffer, &packet);
    printf("[받은 데이터] %s\n", packet.GetData());
}

Client::~Client()
{
    // closesocket()
    closesocket(sock);

    // 윈속 종료
    WSACleanup();
}