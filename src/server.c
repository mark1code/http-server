#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include "base64_decode.h"
#include "server.h"

#define USERNAME "admin"
#define PASSWORD "password"

int server_start(){
	SOCKET socket = create_socket();
	bind_socket(socket);

	listen(socket, 10);
    // Keep listening for clients until server shuts down
    while (1) {
        // Accept a new client connection
        SOCKET client_socket = accept(socket, NULL, NULL);

        // Handle the client request
        handle_request(client_socket);

        // Close the client socket after handling the request
        closesocket(client_socket);
    }
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

int handle_request(SOCKET client) {
    char request[1024] = {0};
    recv(client, request, sizeof(request) - 1, 0);

    // Check for Basic Authentication header
    const char* auth_header = "Authorization: Basic ";
    char* auth_position = strstr(request, auth_header);

    if (auth_position) {
        auth_position += strlen(auth_header);

        // Find the end of the header line
        char* auth_end = strstr(auth_position, "\r\n");
        if (auth_end) {
            *auth_end = '\0'; // Null-terminate the encoded credentials string

            // Decode Base64
            char decoded_credentials[256] = {0};
            decode_base64(decoded_credentials, auth_position);

            // Check credentials
            char expected_credentials[256] = {0};
            snprintf(expected_credentials, sizeof(expected_credentials), "%s:%s", USERNAME, PASSWORD);
            if (strcmp(decoded_credentials, expected_credentials) == 0) {
                // Correct credentials, serve the requested file
                char* request_line_end = strstr(request, "\r\n");
                if (request_line_end) {
                    *request_line_end = '\0'; // Null-terminate the request line
                }

                char* url_start = strstr(request, "GET ");
                if (url_start) {
                    url_start += 4; // Move past "GET "
                    char* url_end = strstr(url_start, " ");
                    if (url_end) {
                        *url_end = '\0'; // Null-terminate the URL

                        // Map URL to file
                        const char* file_to_serve;
                        if (strcmp(url_start, "/") == 0) {
                            file_to_serve = "index.html";
                        } else if (strcmp(url_start, "/secret") == 0) {
                            file_to_serve = "secret.html";
                        } else {
                            file_to_serve = NULL;
                        }

                        if (file_to_serve) {
                            FILE* f = fopen(file_to_serve, "r");
                            if (f) {
                                char buffer[4096];
                                int bytes_read = fread(buffer, 1, sizeof(buffer), f);
                                fclose(f);

                                // Prepare the response headers
                                char response_headers[256];
                                snprintf(response_headers, sizeof(response_headers),
                                         "HTTP/1.1 200 OK\r\n"
                                         "Content-Type: text/html\r\n"
                                         "Content-Length: %d\r\n"
                                         "Connection: close\r\n"
                                         "\r\n", bytes_read);

                                // Send headers and file content
                                send(client, response_headers, strlen(response_headers), 0);
                                send(client, buffer, bytes_read, 0);
                            } else {
                                // File not found response
                                const char* not_found_response =
                                    "HTTP/1.1 404 Not Found\r\n"
                                    "Content-Type: text/html\r\n"
                                    "Connection: close\r\n"
                                    "\r\n"
                                    "<html><body><h1>404 Not Found</h1></body></html>";

                                send(client, not_found_response, strlen(not_found_response), 0);
                            }
                        } else {
                            // File not found response
                            const char* not_found_response =
                                "HTTP/1.1 404 Not Found\r\n"
                                "Content-Type: text/html\r\n"
                                "Connection: close\r\n"
                                "\r\n"
                                "<html><body><h1>404 Not Found</h1></body></html>";

                            send(client, not_found_response, strlen(not_found_response), 0);
                        }
                    }
                }
            } else {
                // Incorrect credentials
                const char* unauthorized_response =
                    "HTTP/1.1 401 Unauthorized\r\n"
                    "WWW-Authenticate: Basic realm=\"Restricted Area\"\r\n"
                    "Content-Type: text/html\r\n"
                    "Connection: close\r\n"
                    "\r\n"
                    "<html><body><h1>401 Unauthorized</h1></body></html>";

                send(client, unauthorized_response, strlen(unauthorized_response), 0);
            }
        }
    } else {
        // No credentials provided
        const char* unauthorized_response =
            "HTTP/1.1 401 Unauthorized\r\n"
            "WWW-Authenticate: Basic realm=\"Restricted Area\"\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"
            "\r\n"
            "<html><body><h1>401 Unauthorized</h1></body></html>";

        send(client, unauthorized_response, strlen(unauthorized_response), 0);
    }

    closesocket(client);
    return 0;
}
