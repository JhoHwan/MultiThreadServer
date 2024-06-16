#include "Client.h"
#include <iostream>
#include <string>

int main()
{
    Client* client = new Client;

    std::string userId;
    std::cout << "[ID를 입력하세요] ";
    std::cin >> userId;

    client->SetUserId(userId);

    try
    {
        client->Init();
        client->Connect();
    }
    catch (const char* e)
    {
        std::cout << "Error : " << e << std::endl;
        exit(0);
    }

    client->Run();

    delete client;

    return 0;
}
