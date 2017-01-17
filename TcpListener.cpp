#include "TcpListener.h"
#include <thread>


TcpListener::TcpListener()
{
}


TcpListener::TcpListener(int buflen, PSTR portn)
{
	this->defaultport = portn;
	this->bufferLength = buflen;
}

TcpListener::~TcpListener()
{
}

bool TcpListener::Initialize()
{
	// Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;

	// Resolve the server address and port
	iResult = getaddrinfo(NULL, defaultport, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	// Create a SOCKET for connecting to server
	ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("socket failed with error: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}

	freeaddrinfo(result);

	iResult = listen(ListenSocket, SOMAXCONN);
	if (iResult == SOCKET_ERROR) {
		printf("listen failed with error: %d\n", WSAGetLastError());
		closesocket(ListenSocket);
		WSACleanup();
		return 1;
	}


	running = true;
	return true;

	
}

void TcpListener::Stop()
{
	this->running = false;
}

void TcpListener::Listen(std::function<void(SOCKET)> func)
{
	printf("Listening to socket\n");
	SOCKET ClientSocket = INVALID_SOCKET;
	while (running)
	{
		sockaddr_in clientInfo;
		int size = sizeof(clientInfo);
		ClientSocket = accept(ListenSocket, (sockaddr*)&clientInfo, &size);

		if (ClientSocket == INVALID_SOCKET) {
			printf("accept failed with error: %d\n", WSAGetLastError());
			closesocket(ListenSocket);
			WSACleanup();
			return;
		}

		else
		{
			printf("Connected: %s\n", inet_ntoa(clientInfo.sin_addr));
			std::thread thr([&] {func(ClientSocket); });

			if (thr.joinable())
			{
				thr.join();
				closesocket(ClientSocket);
				printf("Disconnected %s\n", inet_ntoa(clientInfo.sin_addr));
			}
		}

	}
	
}



void TcpListener::ShutDown()
{
	closesocket(ListenSocket);
	WSACleanup();
}
