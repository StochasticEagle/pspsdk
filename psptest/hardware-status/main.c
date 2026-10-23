#include <pspkernel.h>
#include <psptest.h>
#include <psppower.h>
#include <pspge.h>
PSP_MODULE_INFO("PSPTEST Hardware Info", 0, 1, 0);
PSPTEST_COVERS(scePowerGetCpuClockFrequencyInt);
PSPTEST_COVERS(scePowerGetBusClockFrequencyInt);
PSPTEST_COVERS(scePowerIsPowerOnline);
PSPTEST_COVERS(scePowerIsBatteryExist);
PSPTEST_COVERS(sceGeEdramGetAddr);
PSPTEST_COVERS(sceGeEdramGetSize);
PSPTEST_TEST(power_clock_queries) {
    int cpu=scePowerGetCpuClockFrequencyInt();
    int bus=scePowerGetBusClockFrequencyInt();
    PSPTEST_ASSERT_TRUE(test,cpu>0 && bus>0);
    PSPTEST_ASSERT_TRUE(test,cpu>=bus);
}
PSPTEST_TEST(power_state_queries) {
    int online=scePowerIsPowerOnline();
    int battery=scePowerIsBatteryExist();
    PSPTEST_ASSERT_TRUE(test,online==0 || online==1);
    PSPTEST_ASSERT_TRUE(test,battery==0 || battery==1);
}
PSPTEST_TEST(ge_memory_queries) {
    void *address=sceGeEdramGetAddr();
    unsigned int size=sceGeEdramGetSize();
    PSPTEST_ASSERT_NOT_NULL(test,address);
    PSPTEST_ASSERT_TRUE(test,size>=1024u*1024u);
}
static const PspTestCase cases[]={ PSPTEST_CASE(power_clock_queries),
    PSPTEST_CASE(power_state_queries), PSPTEST_CASE(ge_memory_queries) };
PSPTEST_MODULE("pspsdk/hardware-status", cases, PSP_THREAD_ATTR_USER, NULL, NULL)
