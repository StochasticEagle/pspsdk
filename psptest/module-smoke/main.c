#include <pspkernel.h>
#include <psptest.h>

PSP_MODULE_INFO("PSPTEST Module Smoke", 0, 1, 0);

PSPTEST_TEST(module_executes) {
    PSPTEST_ASSERT_TRUE(test, 1);
}

static const PspTestCase cases[] = {
    PSPTEST_CASE(module_executes)
};

PSPTEST_MODULE("pspsdk/module-smoke", cases, PSP_THREAD_ATTR_USER)
