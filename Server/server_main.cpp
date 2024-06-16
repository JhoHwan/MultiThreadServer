#include <stdio.h>
#include "Server.h"

int main()
{
    Server server;
 
    server.DebugMode = false;

    try
    {
        server.Init();
        server.Run();
    }
    catch(const char* e)
    {
        server.Log(Server::ELogTypes::Error, e);
    }

    server.DeInit();

    return 0;
}