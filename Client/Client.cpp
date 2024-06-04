#include "Client.h"


Client::Client()
{
}

void Client::Init()
{
    // ���� �ʱ�ȭ
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

    // ������ ������ ���
    while (1) {
        // ������ �Է�
        printf("\n[���� ������] ");
        if (fgets(inputBuffer, 100, stdin) == NULL)
            break;

        // '\n' ���� ����
        int len = static_cast<int>(strlen(inputBuffer));

        if (inputBuffer[len - 1] == '\n')
            inputBuffer[len - 1] = '\0';
        if (strlen(inputBuffer) == 0)
            break;

        PK_DATA packet(inputBuffer);
        SendPacket(packet);

        RecivePacket();
        // ������ �ޱ�
    }
}

void Client::SendPacket(const Packet& packet)
{
    char buffer[50];
    writer(packet, buffer);
    int packetSize = packet.GetSize();
    // ������ ������
    int retval = send(sock, buffer, packetSize, 0);
    if (retval == SOCKET_ERROR) {
        std::cout << "SendPacket Failed" << std::endl;;
        return;
    }
    printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� ���½��ϴ�.\n", retval);
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
    printf("[TCP Ŭ���̾�Ʈ] %d����Ʈ�� �޾ҽ��ϴ�.\n", retval);
    PacketHeader header = reader.ReadHeader(buffer);

    auto func = packetFuncMap[header.type];

    func(buffer);
}

void Client::ProcessPK_DATA(const char* buffer)
{
    PK_DATA packet;
    reader(buffer, &packet);
    printf("[���� ������] %s\n", packet.GetData());
}

Client::~Client()
{
    // closesocket()
    closesocket(sock);

    // ���� ����
    WSACleanup();
}