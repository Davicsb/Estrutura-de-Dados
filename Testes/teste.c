#include "main.h"
#include "CUnity/unity.h"

void setUp() {};
void tearDown() {};

void test_soma(){
    TEST_ASSERT_EQUAL(25, soma(20, 5));
}

void test_soma2(){
    TEST_ASSERT_EQUAL(20, soma(10, 5));
}

int main(){
    UNITY_BEGIN();
    RUN_TEST(test_soma);
    RUN_TEST(test_soma2);
    UNITY_END();
    return 0;
}