#pragma once
#ifndef TCPLISTENER_H
#define TCPLISTENER_H

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <functional>

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")


class TcpListener
{
private:
	int bufferLength = 1024;
	PSTR defaultport = "80";

	WSADATA wsaData;
	int iResult;

public:
	SOCKET ListenSocket = INVALID_SOCKET;

private:
	struct addrinfo *result = NULL;
	struct addrinfo hints;

	int iSendResult;
	char *recvbuf;
	int recvbuflen = 1024;
	bool running = false;
public:
	TcpListener();
	TcpListener(int buflen, PSTR portn);
	~TcpListener();

	void Stop();

	bool Initialize();

	void Listen(std::function<void (SOCKET)> func);

	void ShutDown();

};

#endif