#include <pspkernel.h>
#include <psptest.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
PSP_MODULE_INFO("PSPTEST libc", 0, 1, 0);
PSPTEST_COVERS(memmove);
PSPTEST_COVERS(memcpy);
PSPTEST_COVERS(memset);
PSPTEST_COVERS(memcmp);
PSPTEST_COVERS(malloc);
PSPTEST_COVERS(calloc);
PSPTEST_COVERS(realloc);
PSPTEST_COVERS(free);
PSPTEST_COVERS(snprintf);
PSPTEST_COVERS(strtol);
PSPTEST_COVERS(qsort);
PSPTEST_COVERS(bsearch);
PSPTEST_COVERS(sqrtf);
PSPTEST_TEST(byte_memory) {
    unsigned char a[64], b[64];
    memset(a, 0x5a, sizeof(a));
    memcpy(b, a, sizeof(a));
    PSPTEST_ASSERT_EQ_INT(test, 0, memcmp(a,b,sizeof(a)));
    memmove(b+2,b,62);
    PSPTEST_ASSERT_EQ_INT(test, 0x5a,b[63]);
    PSPTEST_ASSERT_EQ_INT(test, 0x5a,b[2]);
}
PSPTEST_TEST(string_conversions) {
    char out[48], *end = NULL;
    int length = snprintf(out,sizeof(out),"value=%d:%s",42,"psp");
    PSPTEST_ASSERT_EQ_INT(test,12,length);
    PSPTEST_ASSERT_EQ_INT(test,0,strcmp(out,"value=42:psp"));
    PSPTEST_ASSERT_EQ_INT(test,255,strtol("ff",&end,16));
    PSPTEST_ASSERT_TRUE(test,end != NULL && *end == '\0');
    PSPTEST_ASSERT_NOT_NULL(test,strstr("pspdev compiler","compiler"));
}
PSPTEST_TEST(heap_allocation) {
    unsigned char *p = (unsigned char *)calloc(96,1);
    int ok = p != NULL, i;
    for(i=0;ok && i<96;i++) if(p[i] != 0) ok=0;
    if(p != NULL) {
        unsigned char *next;
        for(i=0;i<96;i++) p[i]=(unsigned char)i;
        next=(unsigned char *)realloc(p,192);
        if(next == NULL) { free(p); ok=0; }
        else { p=next; for(i=0;i<96;i++) if(p[i]!=(unsigned char)i) ok=0; free(p); }
    }
    PSPTEST_ASSERT_TRUE(test,ok);
}
static int compare_ints(const void *a, const void *b) {
    int left=*(const int *)a,right=*(const int *)b;
    return (left>right)-(left<right);
}
PSPTEST_TEST(sort_and_search) {
    int a[]={9,2,7,1,5},target=7;
    int *found;
    qsort(a,5,sizeof(a[0]),compare_ints);
    PSPTEST_ASSERT_EQ_INT(test,1,a[0]);
    PSPTEST_ASSERT_EQ_INT(test,9,a[4]);
    found=(int *)bsearch(&target,a,5,sizeof(a[0]),compare_ints);
    PSPTEST_ASSERT_NOT_NULL(test,found);
    PSPTEST_ASSERT_EQ_INT(test,7,*found);
}
PSPTEST_TEST(math_library) {
    volatile float a=81.0f;
    float result=sqrtf(a);
    PSPTEST_ASSERT_TRUE(test,fabsf(result-9.0f)<0.00001f);
    PSPTEST_ASSERT_TRUE(test,isfinite(result));
}
static const PspTestCase cases[] = {
    PSPTEST_CASE(byte_memory), PSPTEST_CASE(string_conversions),
    PSPTEST_CASE(heap_allocation), PSPTEST_CASE(sort_and_search), PSPTEST_CASE(math_library)
};
PSPTEST_MODULE("pspsdk/libc-runtime", cases, PSP_THREAD_ATTR_USER, NULL, NULL)
