#include <stdio.h>
#include <winsock2.h>
#include <windows.h>
#include "base64_decode.h"
#include "server.h"

#define USERNAME "benkenobi"
#define PASSWORD "luke"

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

const char* get_mime_type(const char* file_extension) {
    if (strcmp(file_extension, ".html") == 0) return "text/html";
    if (strcmp(file_extension, ".jpg") == 0 || strcmp(file_extension, ".jpeg") == 0) return "image/jpeg";
    if (strcmp(file_extension, ".png") == 0) return "image/png";
    if (strcmp(file_extension, ".gif") == 0) return "image/gif";
    if (strcmp(file_extension, ".css") == 0) return "text/css";
    if (strcmp(file_extension, ".js") == 0) return "application/javascript";
    return "text/html"; // Default MIME type not used to prevent vulnerability
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

            // Decode Base64 credentials
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
                        char file_to_serve[256];
                        char* file_extension = strrchr(url_start, '.');

                        // If the request is for the root, serve index.html
                        if (strcmp(url_start, "/") == 0) {
                            strcpy(file_to_serve, "index.html");
                            file_extension = ".html"; // Default file extension
                        } else {
                            snprintf(file_to_serve, sizeof(file_to_serve), ".%s", url_start); // Add '.' for relative path
                        }

                        // Determine file extension if not already set
                        if (file_extension == NULL) {
                            strcat(file_to_serve, ".html"); // Append default extension
                            file_extension = ".html";
                        }

                        // Open the file
                        FILE* f = fopen(file_to_serve, "rb");
                        if (f) {
                            // Calculate the file size
                            fseek(f, 0, SEEK_END);
                            long file_size = ftell(f);
                            fseek(f, 0, SEEK_SET); // Rewind to the beginning of the file

                            // Prepare the response headers
                            char response_headers[512];
                            snprintf(response_headers, sizeof(response_headers),
                                    "HTTP/1.1 200 OK\r\n"
                                    "Content-Type: %s\r\n"
                                    "Content-Length: %ld\r\n"
                                    "Connection: close\r\n"
                                    "\r\n", get_mime_type(file_extension), file_size);

                            // Send the headers
                            send(client, response_headers, strlen(response_headers), 0);

                            // Send the file content in chunks
                            char buffer[4096];
                            size_t bytes_read;
                            while ((bytes_read = fread(buffer, 1, sizeof(buffer), f)) > 0) {
                                send(client, buffer, bytes_read, 0);
                            }

                            fclose(f); // Close the file
                        } else {
                            // File not found, respond with 404
                            const char* not_found_response =
                                "HTTP/1.1 404 Not Found\r\n"
                                "Content-Type: text/html\r\n"
                                "Connection: close\r\n"
                                "\r\n"
                                "<html><body><h1>404 Not Found</h1></body></html>";

                            // Send the 404 response
                            send(client, not_found_response, strlen(not_found_response), 0);
                        }
                    }
                }
            } else {
                // Incorrect credentials, respond with 401 Unauthorized
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
        // No credentials provided, respond with 401 Unauthorized
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

