#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pspsdk.h>
#include <pspkernel.h>
#include <pspdebug.h>
#include <pspsyscon.h>
#include <psploadcore.h>
#include <pspiofilemgr.h>
#include <pspdisplay.h>

PSP_MODULE_INFO("kpspident", 0x1006, 1, 0);
PSP_MAIN_THREAD_ATTR(0);


int pspSysconGetBaryonVersion(u32 *baryon) {
    int k1 = pspSdkSetK1(0);
    int err = sceSysconGetBaryonVersion((int*)baryon);
    pspSdkSetK1(k1);
    return err;
}

int pspSysconGetPommelVersion(u32 *pommel) {
    int k1 = pspSdkSetK1(0);
    int err = sceSysconGetPommelVersion((int*)pommel);
    pspSdkSetK1(k1);
    return err;
}

int pspSysregGetTachyonVersion(void) {
    int k1 = pspSdkSetK1(0);
    int err = sceSysregGetTachyonVersion();
    pspSdkSetK1(k1);
    return err;
}

u32 pspGetKirkVersion(void) {
    int k1 = pspSdkSetK1(0);
    sceSysregKirkBusClockEnable();
    sceKernelDelayThread(1000);
    int err = *(u32 *)0xBDE00004;
    pspSdkSetK1(k1);
    return err;
}

u32 pspGetSpockVersion(void) {
    int k1 = pspSdkSetK1(0);
    sceSysregAtaBusClockEnable();
    sceKernelDelayThread(1000);
    int err = *(u32 *)0xBDF00004;
    pspSdkSetK1(k1);
    return err;
}

int module_start(SceSize args, void *argp)
{
    return 0;
}

int module_stop(void)
{
    return 0;
}
