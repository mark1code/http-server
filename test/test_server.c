#include "unity.h"
#include "server.h"

// Set and clean up methods
void setUp(void) {
}
void tearDown(void) {
}

void test_create_socket(void){
	// socket file descriptor
	int fd = create_socket();
	// positive if successful
	TEST_ASSERT_GREATER_THAN(0, fd);
	closesocket(fd);
	WSACleanup();
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create_socket);
    return UNITY_END();
}
