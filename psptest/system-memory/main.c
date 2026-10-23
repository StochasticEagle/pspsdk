#include <pspkernel.h>
#include <pspsysmem.h>
#include <psptest.h>
#include <stdint.h>
#include <string.h>
PSP_MODULE_INFO("PSPTEST System Memory", 0, 1, 0);
PSPTEST_COVERS(sceKernelTotalFreeMemSize);
PSPTEST_COVERS(sceKernelMaxFreeMemSize);
PSPTEST_COVERS(sceKernelAllocPartitionMemory);
PSPTEST_COVERS(sceKernelGetBlockHeadAddr);
PSPTEST_COVERS(sceKernelFreePartitionMemory);
PSPTEST_TEST(memory_totals) {
    SceSize free_total=sceKernelTotalFreeMemSize();
    SceSize free_max=sceKernelMaxFreeMemSize();
    PSPTEST_ASSERT_TRUE(test,free_total>0);
    PSPTEST_ASSERT_TRUE(test,free_max>0);
    PSPTEST_ASSERT_TRUE(test,free_total>=free_max);
}
PSPTEST_TEST(partition_lifecycle) {
    SceUID id=sceKernelAllocPartitionMemory(2,"psptest-memory",PSP_SMEM_Low,4096,NULL);
    void *address=NULL;
    int result=-1,ok=0;
    if(id>=0) {
        address=sceKernelGetBlockHeadAddr(id);
        if(address!=NULL) {
            memset(address,0xa5,4096);
            ok=((unsigned char *)address)[0]==0xa5 && ((unsigned char *)address)[4095]==0xa5;
        }
        result=sceKernelFreePartitionMemory(id);
    }
    PSPTEST_ASSERT_TRUE(test,id>=0);
    PSPTEST_ASSERT_NOT_NULL(test,address);
    PSPTEST_ASSERT_TRUE(test,ok);
    PSPTEST_ASSERT_EQ_INT(test,0,result);
}
static const PspTestCase cases[]={ PSPTEST_CASE(memory_totals), PSPTEST_CASE(partition_lifecycle) };
PSPTEST_MODULE("pspsdk/system-memory", cases, PSP_THREAD_ATTR_USER, NULL, NULL)
