// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#define _WINSOCKAPI_    // stops windows.h including winsock.h

#include "targetver.h"

#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>

#include <Windows.h>
#include <WinSock2.h>	// WSADATA, SOCKET, INVALID_SOCKET
#include <WS2tcpip.h>	// getaddrinfo

#pragma comment (lib, "Ws2_32.lib")
#pragma comment (lib, "Mswsock.lib")
#pragma comment (lib, "AdvApi32.lib")