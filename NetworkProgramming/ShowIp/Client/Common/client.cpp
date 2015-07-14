 //
// Copyright (c) 2015 - Atul Mukker <indiaus@gmail.com>
//
//
// ShowIp Client
//

#include "../../Common/include/bridge.h"
#include "../../Common/include/globals.h"

#define DEFAULT_BUFLEN 512

//
// Initialize the client
//
int initializeClient(char *serverAddr)
{
    struct addrinfo hints, *result, *p;
    int rc;
    char ipstr[INET6_ADDRSTRLEN];
    
    memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;	// Don't care if we use IPv4 or IPv6
	hints.ai_socktype = SOCK_STREAM;
	
	// Resolve the local address and port to be used by the server
	rc = getaddrinfo(serverAddr, NULL, &hints, &result);
	if (rc != 0) {
		fprintf(stderr, "getaddrinfo failed: %s\n", gai_strerror(rc));
		return -1;
	}
    
    printf("IP Addresses for %s:\n\n", serverAddr);
    
    for (p = result; p != NULL; p = p->ai_next) {
        void *addr;
        const char *ipver;
        
        if (p->ai_family == AF_INET) {
            struct sockaddr_in *ipv4 = (struct sockaddr_in *)p->ai_addr;
            addr = &ipv4->sin_addr;
            ipver = "IPv4";
        }
        else {
            struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            addr = &ipv6->sin6_addr;
            ipver = "IPv6";
        }
        // Convert the IP to a string and print it
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
        printf("  %s: %s\n", ipver, ipstr);
    }
    
	freeaddrinfo(result);

	return 0;
}
