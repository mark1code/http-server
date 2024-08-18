#ifndef SERVER_H
#define SERVER_H

#include <winsock2.h>

#define PORT 7567

int server_start();
SOCKET create_socket();
int bind_socket(SOCKET s);
int handle_request(SOCKET s);

#endif // SERVER_H
