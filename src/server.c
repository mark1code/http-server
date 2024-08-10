#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include "server.h"

int server_start(){
	printf("Starting server on port %d...\n", PORT);
	create_socket();
	return 0;
}

SOCKET create_socket(){
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2,2), &wsadata);
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	return s;
}

int bind_socket(SOCKET s){
	struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
	bind(s, (struct sockaddr*)&server_addr, sizeof(server_addr));
	
	return 0;
}

