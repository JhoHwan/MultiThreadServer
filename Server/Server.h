#pragma once

#define _WINSOCK_DEPRECATED_NO_WARNINGS 
#pragma comment(lib,"ws2_32")

#include <winsock2.h>
#include <vector>
#include <thread>
#include <mutex>
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
	void ProcessREQ_CON(const SOCKET& clientSock, const char* buffer);
	void ProcessReq_discon(const SOCKET& clientSock, const char* buffer);
	void ProcessREQ_MOVE(const SOCKET& clientSock, const char* buffer);
	void ProcessCHAT_MESSAGE(const SOCKET& clientSock, const char* buffer);

private:
	unsigned int currentConnectedClient;

	SOCKET serverSocket;

	std::map<EPacketType, std::function<void(const SOCKET&, const char*)>> packetFuncMap;

	std::map<SOCKET, std::string> clientIdMap;
	std::mutex mapMutex;
};