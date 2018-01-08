// Pookie.DayTime.Client.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define DEFAULT_BUFLEN 512
#define DT_PORT "60001"

int _cdecl main(int argc, char **argv)
{
	if (argc != 2) {
		printf("usage: %s server-name\n", argv[0]);
	}

	WSADATA wsaData;
	SOCKET ConnectSocket = INVALID_SOCKET;
	struct addrinfo *result = NULL,
		*ptr = NULL,
		hints;

	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;

	// initialize winsock
	if ((iResult = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	}

	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	// resolve server address and port
	if ((iResult = getaddrinfo(argv[1], DT_PORT, &hints, &result)) != 0) {
		printf("getaddrinfo failed with error: %d\n", iResult);
		WSACleanup();
		return 1;
	}

	for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

		if ((ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol)) == INVALID_SOCKET) {
			printf("socket failed with error: %ld\n", WSAGetLastError());
			WSACleanup();
			return 1;
		}

		if ((iResult = connect(ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen)) == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}

		if ((iResult = shutdown(ConnectSocket, SD_SEND)) == SOCKET_ERROR) {
			printf("shutdown failed with error: %ld\n", WSAGetLastError());
			closesocket(ConnectSocket);
			WSACleanup();
			return 1;
		}

		break;
	}

	freeaddrinfo(result);

	do {

		if ((iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0)) > 0) {
			printf(recvbuf);
		} 
		else if (iResult == 0) {
			printf("Connection closed");
		}
		else {
			printf("recv failed with error: %d\n", WSAGetLastError());
		}
	} while (iResult > 0);

	// cleanup
	closesocket(ConnectSocket);
	WSACleanup();

	return 0;
}

