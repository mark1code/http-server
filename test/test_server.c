#include "unity.h"
#include "server.h"
#include <winsock2.h>

// Set and clean up methods
void setUp(void) {
}
void tearDown(void) {
}

void test_create_socket(void){
	// socket file descriptor
	SOCKET s = create_socket();
	// positive if successful
	TEST_ASSERT_NOT_EQUAL(INVALID_SOCKET, s);
	closesocket(s);
	WSACleanup();
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create_socket);
    return UNITY_END();
}
