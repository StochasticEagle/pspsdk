#include <pspsdk.h>

int main(void)
{
    return pspSdkLoadStartModule("toolchain-smoke.prx", 0) < 0;
}
