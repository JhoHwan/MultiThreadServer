#include "Client.h"
#include <sstream>
#include <cstring>
#include <algorithm> 
#include "ChatWindow.h" 

bool is_digits_only(const std::string& s) {
    return std::all_of(s.begin(), s.end(), ::isdigit);
}

Client::Client() : run(false), display(nullptr)
{
    ZeroMemory(&sock, sizeof(SOCKET));
    display = new ChatWindow();

    packetFuncMap.emplace(EPacketType::PK_DATA, std::bind(&Client::ProcessPK_DATA, this, std::placeholders::_1));
    packetFuncMap.emplace(EPacketType::ack_con, std::bind(&Client::ProcessACK_CON, this, std::placeholders::_1));
    packetFuncMap.emplace(EPacketType::ack_discon, std::bind(&Client::ProcessACK_DISCON, this, std::placeholders::_1));
    packetFuncMap.emplace(EPacketType::ack_move, std::bind(&Client::ProcessACK_MOVE, this, std::placeholders::_1));
    packetFuncMap.emplace(EPacketType::chat_message, std::bind(&Client::ProcessCHAT_MESSAGE, this, std::placeholders::_1));
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
   

    REQ_CON req_con(GetUserId());
    SendPacket(req_con);

    run = true;

    display->init();

    ReciveProcessThread = std::thread(&Client::PacketReciver, this);
    ReciveProcessThread.detach();

    InputProcessThread = std::thread(&Client::InputProcesser, this);
    InputProcessThread.detach();

    //display->DrawMessage("Server connect success!");
}

void Client::Run()
{
    while (run)
    {
        
    }
    REQ_DISCON packet(GetUserId());
    SendPacket(packet);
}

void Client::InputProcesser()
{
    std::string message;
    
    while (run)
    {
        std::getline(std::cin, message);
        display->ClearLine(18);


        if (message == "/exit")
        {
            run = false;
            break;
        }

        else if (message.find("/move") != std::string::npos)
        {
            std::istringstream iss(message);
            std::string a, x, y, z, b;
            iss >> a >> x >> y >> z >> b;
            if (a != "/move") continue;
            if (x.empty() || y.empty() || z.empty() || !b.empty())
            {
                display->DrawMessage("[Error] : Please Enter /move x y z format!");
                continue;
            }
            if (!is_digits_only(x) || !is_digits_only(y) || !is_digits_only(z))
            {
                display->DrawMessage("[Error] : Please enter only numbers in x y z!");
                continue;
            }

            int nx, ny, nz = 0;
            nx = atoi(x.c_str());
            ny = atoi(y.c_str());
            nz = atoi(z.c_str());

            Vector3 pos(nx, ny, nz);
            REQ_MOVE packet(pos);
            SendPacket(packet);
            
            display->DrawMessage("I move to [%d, %d, %d] ", nx, ny, nz);

        }
        else
        {
            CHAT_MESSAGE packet(GetUserId(), message);

            SendPacket(packet);
        }
        
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
    //printf("[TCP 클라이언트] %d바이트를 보냈습니다.\n", retval);
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
            //std::cout << "SendPacket Failed" << std::endl;;
            return;
        }

        //printf("[TCP 클라이언트] %d바이트를 받았습니다.\n", retval);

        PacketHeader header = reader.ReadHeader(buffer);

        auto func = packetFuncMap[header.type];
        if(func != nullptr)
            func(buffer);
    }
}

Client::~Client()
{
    // closesocket()
    closesocket(sock);
    ReciveProcessThread.~thread();
    // 윈속 종료
    WSACleanup();
}