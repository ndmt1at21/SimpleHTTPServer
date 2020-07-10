#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

class SimpleHTTPServer
{
public:
	SimpleHTTPServer(int port = 80);
	~SimpleHTTPServer();
	int start();
	void stop();

protected:
	virtual void clientHandler(SOCKET clientSocket) = 0;
	int Error(const char* msg);

protected:
	int m_nPort;
	bool m_bServerRunning;
	SOCKET m_listenSocket;
};

