#include <pspkernel.h>
#include <psptest.h>

PSP_MODULE_INFO("PSPTEST Module Smoke", 0, 1, 0);

static volatile int module_smoke_value;

static int module_smoke_setup(const PspTestEnvironment *environment) {
    if (environment == NULL || environment->version != PSPTEST_ABI_VERSION || environment->program_path == NULL || environment->root_path == NULL) return -1;
    module_smoke_value = 41;
    return 0;
}

PSPTEST_TEST(module_executes) {
    PSPTEST_ASSERT_EQ_INT(test, 41, module_smoke_value);
    module_smoke_value++;
    PSPTEST_ASSERT_EQ_INT(test, 42, module_smoke_value);
}

static int module_smoke_teardown(const PspTestEnvironment *environment) {
    if (environment == NULL || environment->version != PSPTEST_ABI_VERSION) return -1;
    return module_smoke_value == 42 ? 0 : -1;
}

static const PspTestCase cases[] = {
    PSPTEST_CASE(module_executes)
};

PSPTEST_MODULE("pspsdk/module-smoke", cases, PSP_THREAD_ATTR_USER, module_smoke_setup, module_smoke_teardown)
