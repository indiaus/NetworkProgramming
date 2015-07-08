//
// Client
//
#include "../../Common/include/bridge.h"
#include "../../Common/include/globals.h"

#define DEFAULT_BUFLEN 512

//
// Initialize the server
//
int initializeServer()
{
	struct addrinfo *result = NULL, hints;
	SOCKET ClientSocket;
	int iSendResult;
	char recvbuf[DEFAULT_BUFLEN];
	int recvbuflen = DEFAULT_BUFLEN;

	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;	// Don't care if we use IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE; // use my IP

	// Resolve the local address and port to be used by the server
	int iResult = getaddrinfo(NULL, MYPORT, &hints, &result);
	if (iResult != 0) {
		printf("getaddrinfo failed: %s\n", gai_strerror(iResult));
		return -1;
	}

	SOCKET ListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ListenSocket == INVALID_SOCKET) {
		printf("Error at socket(): %x\n", WSAGetLastError());
		freeaddrinfo(result);
		return -1;
	}

	// Setup the TCP listening socket
	iResult = bind(ListenSocket, result->ai_addr, (int)result->ai_addrlen);
	if (iResult == SOCKET_ERROR) {
		printf("bind failed with error: %x\n", WSAGetLastError());
		freeaddrinfo(result);
		closesocket(ListenSocket);
		return -1;
	}

	freeaddrinfo(result);

	if (listen(ListenSocket, BACKLOG) == SOCKET_ERROR) {
		printf( "Listen failed with error: %x\n", WSAGetLastError() );
		closesocket(ListenSocket);
		return -1;
	}

	// Accept a client socket
	ClientSocket = accept(ListenSocket, NULL, NULL);
	if (ClientSocket == INVALID_SOCKET) {
		printf("accept failed: %x\n", WSAGetLastError());
		closesocket(ListenSocket);
		return -1;
	}

	// No longer need server socket
	closesocket(ListenSocket);

	// Receive until the peer shuts down the connection
	do {

		iResult = recv(ClientSocket, recvbuf, recvbuflen, 0);
		if (iResult > 0) {
			printf("Bytes received: %d\n", iResult);

			// Echo the buffer back to the sender
			iSendResult = send( ClientSocket, recvbuf, iResult, 0 );
			if (iSendResult == SOCKET_ERROR) {
				printf("send failed with error: %x\n", WSAGetLastError());
				closesocket(ClientSocket);
				return -1;
			}
			printf("Bytes sent: %d\n", iSendResult);
		}
		else if (iResult == 0)
			printf("Connection closing...\n");
		else  {
			printf("recv failed with error: %x\n", WSAGetLastError());
			closesocket(ClientSocket);
			return -1;
		}

	} while (iResult > 0);

	// shutdown the connection since we're done
	iResult = shutdown(ClientSocket, SD_SEND);
	if (iResult == SOCKET_ERROR) {
		printf("shutdown failed with error: %x\n", WSAGetLastError());
		closesocket(ClientSocket);
		return -1;
	}

	// cleanup
	closesocket(ClientSocket);

	return 0;
}
