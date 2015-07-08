//
// Server
//
#include "../../Common/include/bridge.h"
#include "../../Common/include/globals.h"

#define DEFAULT_BUFLEN 512

//
// Initialize the server
//
int initializeClient(char *serverAddr)
{
	struct addrinfo *result = NULL, *ptr = NULL, hints;
	SOCKET ConnectSocket;
	char *sendbuf = "this is a test";
	char recvbuf[DEFAULT_BUFLEN];
	int iResult;
	int recvbuflen = DEFAULT_BUFLEN;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;	// Don't care if we use IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE; // use my IP

	// Resolve the local address and port to be used by the server
	iResult = getaddrinfo(serverAddr, MYPORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %s\n", gai_strerror(iResult));
		return -1;
	}

	// Attempt to connect to an address until one succeeds
	ConnectSocket = INVALID_SOCKET;
	for(ptr=result; ptr != NULL ;ptr=ptr->ai_next) {

		// Create a SOCKET for connecting to server
		ConnectSocket = socket(ptr->ai_family, ptr->ai_socktype, 
			ptr->ai_protocol);
		if (ConnectSocket == INVALID_SOCKET) {
			printf("socket failed with error: %x\n", WSAGetLastError());
			return -1;
		}

		// Connect to server.
		iResult = connect( ConnectSocket, ptr->ai_addr, (int)ptr->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			closesocket(ConnectSocket);
			ConnectSocket = INVALID_SOCKET;
			continue;
		}
		break;
	}

	freeaddrinfo(result);

	if (ConnectSocket == INVALID_SOCKET) {
		printf("Unable to connect to server!\n");
		return -1;
	}

	// Send an initial buffer
	iResult = send(ConnectSocket, sendbuf, (int)strlen(sendbuf), 0);
	if (iResult == SOCKET_ERROR) {
		printf("send failed with error: %x\n", WSAGetLastError());
		closesocket(ConnectSocket);
		return -1;
	}

	printf("Bytes Sent: %x\n", iResult);

	// shutdown the connection since no more data will be sent
	iResult = shutdown(ConnectSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %x\n", WSAGetLastError());
		closesocket(ConnectSocket);
		return -1;
	}

	// Receive until the peer closes the connection
	do {
		iResult = recv(ConnectSocket, recvbuf, recvbuflen, 0);
		if ( iResult > 0 )
			printf("Bytes received: %d\n", iResult);
		else if ( iResult == 0 )
			printf("Connection closed\n");
		else
			printf("recv failed with error: %x\n", WSAGetLastError());

	} while( iResult > 0 );

	// cleanup
	closesocket(ConnectSocket);

	return 0;
}
