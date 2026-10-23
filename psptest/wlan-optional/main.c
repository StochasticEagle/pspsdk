#include <pspkernel.h>
#include <psptest.h>
#include <pspwlan.h>
#include <stdint.h>

PSP_MODULE_INFO("PSPTEST Optional WLAN", 0, 1, 0);

PSPTEST_COVERS(sceWlanDevIsPowerOn);
PSPTEST_COVERS(sceWlanGetSwitchState);
PSPTEST_COVERS(sceWlanGetEtherAddr);

PSPTEST_TEST(wlan_switch_state) {
    int state = sceWlanGetSwitchState();
    if (state < 0) {
        psptest_skip(test, "WLAN switch API unavailable on this device");
        return;
    }
    PSPTEST_ASSERT_TRUE(test, state == 0 || state == 1);
}

PSPTEST_TEST(wlan_controller_address) {
    uint8_t address[8] = {0};
    int power = sceWlanDevIsPowerOn();
    int status;
    if (power <= 0) {
        psptest_skip(test, "WLAN controller unavailable or powered off");
        return;
    }
    status = sceWlanGetEtherAddr(address);
    PSPTEST_ASSERT_EQ_INT(test, 0, status);
}

static const PspTestCase cases[] = {
    PSPTEST_CASE(wlan_switch_state),
    PSPTEST_CASE(wlan_controller_address)
};

PSPTEST_MODULE("pspsdk/wlan-optional", cases, PSP_THREAD_ATTR_USER, NULL, NULL)
