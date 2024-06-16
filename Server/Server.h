#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#pragma comment(lib,"ws2_32")

#include <winsock2.h>
#include <vector>
#include <thread>
#include <map>
#include <functional>

#define SERVERPORT 9000
#define BUFSIZE 512


class Packet;
enum class EPacketType : char;

class Server
{
public:
	enum class ELogTypes
	{
		Log,
		Debug,
		Error,
		Invaild
	};

	Server();

	void Init();
	void Run();
	void DeInit();

	void ProcessClient(const SOCKET clientSocket);
	void Log(ELogTypes inLogType, const char* inLog, ...) const;

	bool DebugMode;

private:
	void SendPacket(const SOCKET& clientSock, const Packet& packet);

	void CreateClientThread(const SOCKET& clientSock);
	void ProcessPacket(const SOCKET& clientSock, const char* buffer);
	void ProcessPK_DATA(const SOCKET& clientSock, const char* buffer);
	void ProcessReq_con(const SOCKET& clientSock, const char* buffer);
	void ProcessReq_discon(const SOCKET& clientSock, const char* buffer);
	void ProcessReq_move(const SOCKET& clientSock, const char* buffer);

private:
	unsigned int currentConnectedClient;

	SOCKET serverSocket;

	std::map<EPacketType, std::function<void(const SOCKET&, const char*)>> packetFuncMap;

	std::vector<SOCKET> clientList;
};