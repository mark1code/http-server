#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include "server.h"

int server_start(){
	printf("hello server");
	create_socket();
	return 0;
}

SOCKET create_socket(){
	WSADATA wsadata;
	WSAStartup(MAKEWORD(2,2), &wsadata);
	SOCKET s = socket(AF_INET, SOCK_STREAM, 0);
	return s;
}


