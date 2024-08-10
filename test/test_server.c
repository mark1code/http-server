#include "unity.h"
#include "server.h"
#include <winsock2.h>

// Set and clean up methods
void setUp(void) {
}
void tearDown(void) {
}

void test_create_socket(void){
	SOCKET s = create_socket();
	TEST_ASSERT_NOT_EQUAL(INVALID_SOCKET, s);
	closesocket(s);
	WSACleanup();
}

void test_bind_socket(void){
	SOCKET s = create_socket();
	TEST_ASSERT_NOT_EQUAL(INVALID_SOCKET, s);

	int res = bind_socket(s);
	TEST_ASSERT_EQUAL(0, res);

	closesocket(s);
	WSACleanup();
}

void test_server_start(void){
	// server start integration test
	TEST_ASSERT_EQUAL(0, server_start());
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create_socket);
    RUN_TEST(test_bind_socket);
    return UNITY_END();
}
