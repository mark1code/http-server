#ifndef SERVER_H
#define SERVER_H
#define PORT 8080

#include <winsock2.h>

int server_start();
SOCKET create_socket();
int bind_socket(SOCKET s);
int handle_request(SOCKET s);

#endif // SERVER_H
