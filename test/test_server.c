#include "unity.h"
#include "server.h"

// Set and clean up methods
void setUp(void) {
}
void tearDown(void) {
}

// Example test
void test_add_should_return_sum_of_two_numbers(void) {
    int result = add(1, 2);
    TEST_ASSERT_EQUAL(3, result);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_add_should_return_sum_of_two_numbers);
    return UNITY_END();
}
