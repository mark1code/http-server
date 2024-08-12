#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include "server.h"

int server_start(){
	printf("Starting server on port %d...\n", PORT);
	SOCKET socket = create_socket();
	bind_socket(socket);
	client_accept(socket);
	closesocket(socket);
    WSACleanup();
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


int client_accept(SOCKET s){
	listen(s,10);
	SOCKET client = accept(s, 0, 0);
	char request[1024] = {0};
	recv(client, request, sizeof(request) - 1, 0);
	// Memory compare
	if (memcmp(request, "GET / ", 6) == 0) {
		FILE* f = fopen("index.html", "r");
		if (f) {
			char buffer[1024];
			int bytes_read = fread(buffer, 1, sizeof(buffer), f);
			fclose(f);

			// HTTP response headers
			const char* response_headers =
				"HTTP/1.1 200 OK\r\n"
				"Content-Type: text/html\r\n"
				"Connection: close\r\n"
				"\r\n";

			send(client, response_headers, strlen(response_headers), 0);
			send(client, buffer, bytes_read, 0);
		} else {
			const char* not_found_response =
				"HTTP/1.1 404 Not Found\r\n"
				"Content-Type: text/html\r\n"
				"Connection: close\r\n"
				"\r\n"
				"<html><body><h1>404 Not Found</h1></body></html>";

			send(client, not_found_response, strlen(not_found_response), 0);
		}
	}


	return 0;
}

