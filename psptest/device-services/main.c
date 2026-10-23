#include <pspkernel.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <psprtc.h>
#include <psptest.h>
PSP_MODULE_INFO("PSPTEST Device Services", 0, 1, 0);
PSPTEST_COVERS(sceDisplayGetMode);
PSPTEST_COVERS(sceDisplayGetFramePerSec);
PSPTEST_COVERS(sceCtrlPeekBufferPositive);
PSPTEST_COVERS(sceRtcGetTickResolution);
PSPTEST_COVERS(sceRtcGetCurrentTick);
PSPTEST_COVERS(sceRtcGetCurrentClockLocalTime);
PSPTEST_TEST(display_mode) {
    int mode=0,width=0,height=0;
    int status=sceDisplayGetMode(&mode,&width,&height);
    PSPTEST_ASSERT_EQ_INT(test,0,status);
    PSPTEST_ASSERT_TRUE(test,width>0 && height>0);
    PSPTEST_ASSERT_TRUE(test,sceDisplayGetFramePerSec()>0.0f);
}
PSPTEST_TEST(controller_snapshot) {
    SceCtrlData pad={0};
    int count=sceCtrlPeekBufferPositive(&pad,1);
    PSPTEST_ASSERT_EQ_INT(test,1,count);
}
PSPTEST_TEST(rtc_clock) {
    u64 tick=0;
    ScePspDateTime when={0};
    int a=sceRtcGetCurrentTick(&tick);
    int b=sceRtcGetCurrentClockLocalTime(&when);
    PSPTEST_ASSERT_TRUE(test,sceRtcGetTickResolution()>0);
    PSPTEST_ASSERT_EQ_INT(test,0,a);
    PSPTEST_ASSERT_EQ_INT(test,0,b);
    PSPTEST_ASSERT_TRUE(test,tick>0);
    PSPTEST_ASSERT_TRUE(test,when.month>=1 && when.month<=12);
}
static const PspTestCase cases[]={ PSPTEST_CASE(display_mode),
    PSPTEST_CASE(controller_snapshot), PSPTEST_CASE(rtc_clock) };
PSPTEST_MODULE("pspsdk/device-services", cases, PSP_THREAD_ATTR_USER, NULL, NULL)
