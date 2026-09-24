#include <pspkernel.h>
#include <psptest.h>

PSP_MODULE_INFO("PSPTEST Framework", 0, 1, 0);
PSP_MAIN_THREAD_ATTR(PSP_THREAD_ATTR_USER);

PSPTEST_TEST(assertions_work) {
    int value = 42;
    PSPTEST_ASSERT_TRUE(test, value > 0);
    PSPTEST_ASSERT_EQ_INT(test, 42, value);
    PSPTEST_ASSERT_NOT_NULL(test, &value);
}

PSPTEST_TEST(skip_is_recorded) {
    psptest_skip(test, "framework skip-path self-test");
}

static const PspTestCase cases[] = {
    PSPTEST_CASE(assertions_work),
    PSPTEST_CASE(skip_is_recorded)
};

PSPTEST_MAIN("pspsdk/framework", cases)
