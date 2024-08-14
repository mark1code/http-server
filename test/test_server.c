#include "unity.h"
#include "server.h"
#include <winsock2.h>
#include <stdio.h>
#include "base64_decode.h"

// Set up and clean up
void setUp(void) {
    // Initialise Winsock 
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);
}

void tearDown(void) {
    // Clean up Winsock
    WSACleanup();
}

// Socket creation test
void test_create_socket(void){
	SOCKET s = create_socket();
	TEST_ASSERT_NOT_EQUAL(INVALID_SOCKET, s);
	closesocket(s);
	WSACleanup();
}

// Bind socket test
void test_bind_socket(void){
	SOCKET s = create_socket();
	TEST_ASSERT_NOT_EQUAL(INVALID_SOCKET, s);

	int res = bind_socket(s);
	TEST_ASSERT_EQUAL(0, res);

	closesocket(s);
	WSACleanup();
}

// Start server in a separate thread
DWORD WINAPI start_server() {
    server_start();
    return 0;
}

// Handling client request test
void test_client_accept(void) {
    HANDLE serverThread;
    SOCKET clientSocket;
    struct sockaddr_in server_addr;
    char response[1024] = {0};
    int bytes_received;

    // Start server thread
    serverThread = CreateThread(NULL, 0, start_server, NULL, 0, NULL);

    // Create client socket
    WSADATA wsadata;
    WSAStartup(MAKEWORD(2, 2), &wsadata);
    clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    // Connect to server
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    server_addr.sin_port = htons(PORT);
    connect(clientSocket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    // Send HTTP request
    const char* request_message = "GET / HTTP/1.1\r\nHost: localhost\r\n\r\n";
    send(clientSocket, request_message, strlen(request_message), 0);

    // Receive response
    bytes_received = recv(clientSocket, response, sizeof(response) - 1, 0);
    response[bytes_received] = '\0';  // Ensure correct format

    // Check response, no details provided so send
    const char* expected_response =
    "HTTP/1.1 401 Unauthorized\r\n"
    "WWW-Authenticate: Basic realm=\"Restricted Area\"\r\n"
    "Content-Type: text/html\r\n"
    "Connection: close\r\n"
    "\r\n";
    TEST_ASSERT_EQUAL_STRING(expected_response, response);

    // Clean up
    closesocket(clientSocket);
    WaitForSingleObject(serverThread, 100);
    CloseHandle(serverThread);
}

// Decoding base64 test
void test_decode_b64(void){
    char encoded[] = "YWRtaW46cGFzc3dvcmQ="; // Base64 for "admin:password"
    char decoded[256];

    decode_base64(decoded, encoded);

    TEST_ASSERT_EQUAL_STRING("admin:password", decoded);
}

void run_tests(void){
	RUN_TEST(test_create_socket);
    RUN_TEST(test_bind_socket);
    RUN_TEST(test_client_accept);
    RUN_TEST(test_decode_b64);
}

int main(void) {
    UNITY_BEGIN();
	run_tests();
    return UNITY_END();
}
