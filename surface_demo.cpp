#include <cutils/memory.h>

#include <utils/Log.h>

#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>

#include <gui/Surface.h>
#include <gui/SurfaceComposerClient.h>
#include <android/native_window.h>

using namespace android;

int main(int argc, char** argv)
{
    // set up the thread-pool
    sp<ProcessState> proc(ProcessState::self()); //binder获取surfaceFlinger服务
    ProcessState::self()->startThreadPool();

    // create a client to surfaceflinger
    sp<SurfaceComposerClient> client = new SurfaceComposerClient();
    sp<SurfaceControl> surfaceControl = client->createSurface(String8("surfaceTest"),
                                                              160, 240, PIXEL_FORMAT_RGB_565, 0);

    sp<Surface> surface = surfaceControl->getSurface();

    ANativeWindow_Buffer outBuffer;
    surface->lock(&outBuffer, NULL);
    ssize_t bpr = outBuffer.stride * bytesPerPixel(outBuffer.format);
    android_memset16((uint16_t*)outBuffer.bits, 0x0FFF, bpr*outBuffer.height / 2);
    android_memset16((uint16_t*)outBuffer.bits + bpr*outBuffer.height / 2 / 2, 0x0AF0, bpr*outBuffer.height / 2);
    ALOGD("bpr = %d; outBuffer.height = %d;\n", bpr, outBuffer.height);
    surface->unlockAndPost();

    IPCThreadState::self()->joinThreadPool();

    return 0;
}