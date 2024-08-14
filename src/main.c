#include <stdio.h>
#include "server.h"

int main(){
	printf("Starting server on port %d...\n", PORT);
	server_start();
	return 0;
}
