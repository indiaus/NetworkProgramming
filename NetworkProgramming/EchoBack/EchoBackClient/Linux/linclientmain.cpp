//
// Copyright (c) 2015 - Atul Mukker <indiaus@gmail.com>
//

#include <stdio.h>

#include "../../Common/include/exports.h"

int main(int argc, char *argv[])
{
	// Validate the parameters
	if (argc != 2) {
		printf("usage: %s server-name\n", argv[0]);
		return 1;
	}
	if (initializeClient(argv[1]) == -1) {
		printf("Could not initialize the server\n");
		return 1;
	}

	return 0;
}
