#include <stdio.h>

#include "../../Common/include/exports.h"

int main(int argc, char *argv[])
{
	if (initializeServer() == -1) {
		printf("Could not initialize the server\n");
		return 1;
	}

	return 0;
}
