#include "unity.h"
#include "server.h"
#include <winsock2.h>
#include <stdio.h>

// Set and clean up methods
void setUp(void) {
}
void tearDown(void) {
}

// Socket creation
void test_create_socket(void){
	SOCKET s = create_socket();
	TEST_ASSERT_NOT_EQUAL(INVALID_SOCKET, s);
	closesocket(s);
	WSACleanup();
}

// Bind socket
void test_bind_socket(void){
	SOCKET s = create_socket();
	TEST_ASSERT_NOT_EQUAL(INVALID_SOCKET, s);

	int res = bind_socket(s);
	TEST_ASSERT_EQUAL(0, res);

	closesocket(s);
	WSACleanup();
}


// Listening and accepting a client connection
void test_listen_and_accept_socket(void) {
    SOCKET s = create_socket();
    TEST_ASSERT_NOT_EQUAL(INVALID_SOCKET, s);
	

    int bind_res = bind_socket(s);
    TEST_ASSERT_EQUAL(0, bind_res);

    // Start a client connection as separate process
    SOCKET client_socket = create_socket();
    struct sockaddr_in client_addr;
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Localhost
    client_addr.sin_port = htons(PORT);
    
    int conn_res = connect(client_socket, (struct sockaddr*)&client_addr, sizeof(client_addr));
    TEST_ASSERT_EQUAL(0, conn_res);

    // Server should accept this connection
    SOCKET client = accept(s, NULL, NULL);
    TEST_ASSERT_NOT_EQUAL(INVALID_SOCKET, client);

    closesocket(client);
    closesocket(client_socket);
    closesocket(s);
    WSACleanup();
}

// Test for receiving data from a client
void test_recv_data(void) {
    SOCKET s = create_socket();
    TEST_ASSERT_NOT_EQUAL(INVALID_SOCKET, s);

    int bind_res = bind_socket(s);
    TEST_ASSERT_EQUAL(0, bind_res);

    int listen_res = listen(s, 10);
    TEST_ASSERT_EQUAL(0, listen_res);

    // Simulate a client connection here and send data
    SOCKET client = accept(s, NULL, NULL);
    TEST_ASSERT_NOT_EQUAL(INVALID_SOCKET, client);


    char request[256] = {0};
    int recv_res = recv(client, request, sizeof(request), 0);
    TEST_ASSERT_GREATER_THAN(0, recv_res); // Data received
    TEST_ASSERT_NOT_EQUAL(0, strlen(request)); // Request not empty

    closesocket(client);
    closesocket(s);
    WSACleanup();
}


void test_server_start(void){
	// Server start integration test
	TEST_ASSERT_EQUAL(0, server_start());
}

void run_tests(void){
	RUN_TEST(test_create_socket);
    RUN_TEST(test_bind_socket);
	RUN_TEST(test_listen_and_accept_socket);
	RUN_TEST(test_recv_data);

	RUN_TEST(test_server_start);
}

int main(void) {
    UNITY_BEGIN();
	run_tests();
    return UNITY_END();
}
