#include "Client.h"

int main()
{
    Client client;

    try
    {
        client.Init();
        client.Connect();
    }
    catch (const char* e)
    {
        std::cout << "Error : " << e << std::endl;
        exit(0);
    }

    client.Run();

    return 0;
}
