#include <pspkernel.h>
#include <psptest.h>
#include <pspthreadman.h>
#include <stdint.h>
PSP_MODULE_INFO("PSPTEST Kernel Sync", 0, 1, 0);
PSPTEST_COVERS(sceKernelCreateSema);
PSPTEST_COVERS(sceKernelPollSema);
PSPTEST_COVERS(sceKernelSignalSema);
PSPTEST_COVERS(sceKernelWaitSema);
PSPTEST_COVERS(sceKernelDeleteSema);
PSPTEST_COVERS(sceKernelCreateEventFlag);
PSPTEST_COVERS(sceKernelSetEventFlag);
PSPTEST_COVERS(sceKernelPollEventFlag);
PSPTEST_COVERS(sceKernelDeleteEventFlag);
PSPTEST_COVERS(sceKernelCreateThread);
PSPTEST_COVERS(sceKernelStartThread);
PSPTEST_COVERS(sceKernelWaitThreadEnd);
PSPTEST_COVERS(sceKernelDeleteThread);
PSPTEST_COVERS(sceKernelGetSystemTimeWide);
PSPTEST_COVERS(sceKernelDelayThread);
PSPTEST_TEST(semaphore_lifecycle) {
    SceUID id=sceKernelCreateSema("pt-sema",0,1,2,NULL);
    int a=-1,b=-1,c=-1,d=-1;
    if(id>=0) {
        a=sceKernelPollSema(id,1);
        b=sceKernelSignalSema(id,1);
        c=sceKernelWaitSema(id,1,NULL);
        d=sceKernelDeleteSema(id);
    }
    PSPTEST_ASSERT_TRUE(test,id>=0);
    PSPTEST_ASSERT_EQ_INT(test,0,a);
    PSPTEST_ASSERT_EQ_INT(test,0,b);
    PSPTEST_ASSERT_EQ_INT(test,0,c);
    PSPTEST_ASSERT_EQ_INT(test,0,d);
}
PSPTEST_TEST(event_flags) {
    SceUID id=sceKernelCreateEventFlag("pt-event",0,0,NULL);
    unsigned int bits=0;
    int a=-1,b=-1,c=-1;
    if(id>=0) {
        a=sceKernelSetEventFlag(id,0x14);
        b=sceKernelPollEventFlag(id,0x10,0,&bits);
        c=sceKernelDeleteEventFlag(id);
    }
    PSPTEST_ASSERT_TRUE(test,id>=0);
    PSPTEST_ASSERT_EQ_INT(test,0,a);
    PSPTEST_ASSERT_EQ_INT(test,0,b);
    PSPTEST_ASSERT_TRUE(test,(bits&0x10u)!=0);
    PSPTEST_ASSERT_EQ_INT(test,0,c);
}
static volatile int child_ran;
static int child_entry(SceSize args,void *argp) {
    (void)args; (void)argp;
    child_ran=1;
    return 0;
}
PSPTEST_TEST(thread_lifecycle) {
    SceUID id;
    int a=-1,b=-1,c=-1;
    child_ran=0;
    id=sceKernelCreateThread("pt-child",child_entry,32,16*1024,PSP_THREAD_ATTR_USER,NULL);
    if(id>=0) {
        a=sceKernelStartThread(id,0,NULL);
        if(a>=0) b=sceKernelWaitThreadEnd(id,NULL);
        c=sceKernelDeleteThread(id);
    }
    PSPTEST_ASSERT_TRUE(test,id>=0);
    PSPTEST_ASSERT_EQ_INT(test,0,a);
    PSPTEST_ASSERT_EQ_INT(test,0,b);
    PSPTEST_ASSERT_EQ_INT(test,0,c);
    PSPTEST_ASSERT_EQ_INT(test,1,child_ran);
}
PSPTEST_TEST(monotonic_clock) {
    SceInt64 before=sceKernelGetSystemTimeWide();
    int delay=sceKernelDelayThread(1000);
    SceInt64 after=sceKernelGetSystemTimeWide();
    PSPTEST_ASSERT_EQ_INT(test,0,delay);
    PSPTEST_ASSERT_TRUE(test,after>=before);
}
static const PspTestCase cases[]={ PSPTEST_CASE(semaphore_lifecycle),
    PSPTEST_CASE(event_flags), PSPTEST_CASE(thread_lifecycle), PSPTEST_CASE(monotonic_clock) };
PSPTEST_MODULE("pspsdk/kernel-sync", cases, PSP_THREAD_ATTR_USER, NULL, NULL)
