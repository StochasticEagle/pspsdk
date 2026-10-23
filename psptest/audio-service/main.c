#include <pspkernel.h>
#include <psptest.h>
#include <pspaudio.h>
PSP_MODULE_INFO("PSPTEST Audio Services", 0, 1, 0);
PSPTEST_COVERS(sceAudioChReserve);
PSPTEST_COVERS(sceAudioGetChannelRestLen);
PSPTEST_COVERS(sceAudioChangeChannelVolume);
PSPTEST_COVERS(sceAudioChRelease);
PSPTEST_TEST(audio_channel_lifecycle) {
    int channel=sceAudioChReserve(-1,64,PSP_AUDIO_FORMAT_STEREO);
    int queued=-1,volume=-1,released=-1;
    if(channel>=0) {
        queued=sceAudioGetChannelRestLen(channel);
        volume=sceAudioChangeChannelVolume(channel,0,0);
        released=sceAudioChRelease(channel);
    }
    PSPTEST_ASSERT_TRUE(test,channel>=0);
    PSPTEST_ASSERT_TRUE(test,queued>=0);
    PSPTEST_ASSERT_EQ_INT(test,0,volume);
    PSPTEST_ASSERT_EQ_INT(test,0,released);
}
static const PspTestCase cases[]={ PSPTEST_CASE(audio_channel_lifecycle) };
PSPTEST_MODULE("pspsdk/audio-service", cases, PSP_THREAD_ATTR_USER, NULL, NULL)
