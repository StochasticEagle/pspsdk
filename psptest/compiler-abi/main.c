#include <pspkernel.h>
#include <psptest.h>
#include <stdint.h>
#include <limits.h>
PSP_MODULE_INFO("PSPTEST Compiler ABI", 0, 1, 0);
_Static_assert(sizeof(void*) == 4, "PSP O32 pointer width");
_Static_assert(sizeof(int) == 4, "PSP int width");
_Static_assert(sizeof(long) == 4, "PSP O32 long width");
_Static_assert(sizeof(long long) == 8, "64-bit integer ABI");
_Static_assert(sizeof(float) == 4 && sizeof(double) == 8, "floating point ABI");
PSPTEST_COVERS(__builtin_bswap32);
PSPTEST_COVERS(__builtin_allegrex_bitrev);
PSPTEST_COVERS(__builtin_allegrex_clo);
PSPTEST_COVERS(__builtin_allegrex_ctz);
PSPTEST_COVERS(__sync_fetch_and_add);
PSPTEST_TEST(integer_arithmetic) {
    volatile int a = -13, b = 5;
    volatile unsigned int u = 0x12345678u;
    PSPTEST_ASSERT_EQ_INT(test, -2, a / b);
    PSPTEST_ASSERT_EQ_INT(test, -3, a % b);
    PSPTEST_ASSERT_EQ_INT(test, 0x78563412u, __builtin_bswap32(u));
    PSPTEST_ASSERT_EQ_INT(test, 31, __builtin_clz(1u));
}
PSPTEST_TEST(allegrex_instructions) {
    volatile unsigned int one = 1u, mask = 0xffffffffu, v = 32u;
    PSPTEST_ASSERT_EQ_INT(test, 0x80000000u, __builtin_allegrex_bitrev(one));
    PSPTEST_ASSERT_EQ_INT(test, 32, __builtin_allegrex_clo(mask));
    PSPTEST_ASSERT_EQ_INT(test, 5, __builtin_allegrex_ctz(v));
}
PSPTEST_TEST(software_64bit_helpers) {
    volatile uint64_t n = 0x123456789abcdef0ULL, d = 1000003ULL;
    uint64_t q = n / d, r = n % d;
    PSPTEST_ASSERT_TRUE(test, q > 0);
    PSPTEST_ASSERT_TRUE(test, r < d);
    PSPTEST_ASSERT_TRUE(test, q * d + r == n);
}
PSPTEST_TEST(software_double_arithmetic) {
    volatile double a = 1.25, b = 2.5;
    double c = a + b;
    PSPTEST_ASSERT_TRUE(test, c == 3.75);
    PSPTEST_ASSERT_TRUE(test, (b / a) == 2.0);
}
PSPTEST_TEST(atomic_word_ops) {
    volatile int value = 4;
    PSPTEST_ASSERT_EQ_INT(test, 4, __sync_fetch_and_add(&value, 3));
    PSPTEST_ASSERT_EQ_INT(test, 7, value);
    PSPTEST_ASSERT_TRUE(test, __sync_bool_compare_and_swap(&value, 7, 11));
    PSPTEST_ASSERT_EQ_INT(test, 11, value);
}
static const PspTestCase cases[] = {
    PSPTEST_CASE(integer_arithmetic), PSPTEST_CASE(allegrex_instructions),
    PSPTEST_CASE(software_64bit_helpers), PSPTEST_CASE(software_double_arithmetic),
    PSPTEST_CASE(atomic_word_ops)
};
#ifndef PSPTEST_COMPILER_SUITE
#define PSPTEST_COMPILER_SUITE "pspsdk/compiler-abi"
#endif
PSPTEST_MODULE(PSPTEST_COMPILER_SUITE, cases, PSP_THREAD_ATTR_USER, NULL, NULL)
