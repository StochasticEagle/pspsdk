#include <pspkernel.h>
#include <pspiofilemgr.h>
#include <psptest.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
PSP_MODULE_INFO("PSPTEST File IO", 0, 1, 0);
PSPTEST_COVERS(sceIoOpen);
PSPTEST_COVERS(sceIoClose);
PSPTEST_COVERS(sceIoWrite);
PSPTEST_COVERS(sceIoRead);
PSPTEST_COVERS(sceIoLseek);
PSPTEST_COVERS(sceIoRemove);
static char test_file[320];
static int setup(const PspTestEnvironment *env) {
    int size;
    if(env==NULL || env->root_path==NULL) return -1;
    size=snprintf(test_file,sizeof(test_file),"%s/results/psptest-file-io.tmp",env->root_path);
    return size>0 && (size_t)size<sizeof(test_file) ? 0 : -1;
}
static int teardown(const PspTestEnvironment *env) {
    (void)env;
    sceIoRemove(test_file);
    return 0;
}
PSPTEST_TEST(file_round_trip) {
    static const char data[]="PSPTEST filesystem payload";
    char buffer[sizeof(data)]={0};
    SceUID fd=sceIoOpen(test_file,O_CREAT|O_TRUNC|O_RDWR,0777);
    int written=-1,read_count=-1,closed=-1;
    SceOff pos=-1;
    if(fd>=0) {
        written=sceIoWrite(fd,data,sizeof(data));
        pos=sceIoLseek(fd,0,SEEK_SET);
        if(pos==0) read_count=sceIoRead(fd,buffer,sizeof(buffer));
        closed=sceIoClose(fd);
    }
    PSPTEST_ASSERT_TRUE(test,fd>=0);
    PSPTEST_ASSERT_EQ_INT(test,sizeof(data),written);
    PSPTEST_ASSERT_EQ_INT(test,0,pos);
    PSPTEST_ASSERT_EQ_INT(test,sizeof(buffer),read_count);
    PSPTEST_ASSERT_EQ_INT(test,0,memcmp(data,buffer,sizeof(data)));
    PSPTEST_ASSERT_EQ_INT(test,0,closed);
}
PSPTEST_TEST(remove_file) {
    int result=sceIoRemove(test_file);
    PSPTEST_ASSERT_EQ_INT(test,0,result);
}
static const PspTestCase cases[]={ PSPTEST_CASE(file_round_trip), PSPTEST_CASE(remove_file) };
PSPTEST_MODULE("pspsdk/file-io", cases, PSP_THREAD_ATTR_USER, setup, teardown)
