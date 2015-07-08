//
// This file is an attempt to create a bridge so that we can run the server and client
// on windows and other OSes
//

#if defined(MYAPP_OS_WINDOWS)

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>

#elif defined(MYAPP_OS_LINUX) || defined(MYAPP_OS_MACOSX)

#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>

typedef int SOCKET;
#define INVALID_SOCKET	(-1)
#define SOCKET_ERROR    (-1)

#define closesocket close
#define WSAGetLastError() errno

#define SD_RECEIVE SHUT_RD
#define SD_SEND SHUT_WR
#define SD_BOTH SHUT_RDWR

#else

#error "OS Not supported"

#endif

