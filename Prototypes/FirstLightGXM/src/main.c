/*
 * FIRST LIGHT — DIRECT SceGxm DISPLAY BRING-UP
 *
 * This is the first direct-GXM milestone: it initializes SceGxm, allocates
 * two CDRAM display buffers, creates color surfaces and sync objects, and
 * presents animated CPU-generated test patterns through the GXM display queue.
 *
 * IMPORTANT: This is NOT yet the matching 3D cube benchmark. It validates
 * direct GXM initialization/presentation only. Do not compare its FPS against
 * the vitaGL cube as a performance result. The next milestone is a shared
 * shader-backed cube renderer using sceGxmDraw.
 *
 * The display/synchronization sequence follows VitaSDK's public ime sample.
 */
#include <psp2/types.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/kernel/sysmem.h>
#include <psp2/display.h>
#include <psp2/gxm.h>
#include <psp2/ctrl.h>
#include <stdint.h>
#include <string.h>

#define DISPLAY_WIDTH 960
#define DISPLAY_HEIGHT 544
#define DISPLAY_STRIDE 1024
#define BUFFER_COUNT 2
#define MAX_PENDING_SWAPS 1
#define ALIGN_UP(v, a) (((v) + ((a) - 1)) & ~((a) - 1))

typedef struct DisplayBuffer {
    void *data;
    SceUID memblock;
    SceGxmSyncObject *sync;
    SceGxmColorSurface surface;
} DisplayBuffer;

static DisplayBuffer buffers[BUFFER_COUNT];
static unsigned int back_index = 0;
static unsigned int front_index = 1;

static void display_callback(const void *callback_data)
{
    void *address = *(void * const *)callback_data;
    SceDisplayFrameBuf frame = {
        .size = sizeof(SceDisplayFrameBuf),
        .base = address,
        .pitch = DISPLAY_STRIDE,
        .pixelformat = SCE_DISPLAY_PIXELFORMAT_A8B8G8R8,
        .width = DISPLAY_WIDTH,
        .height = DISPLAY_HEIGHT
    };
    sceDisplaySetFrameBuf(&frame, SCE_DISPLAY_SETBUF_NEXTFRAME);
}

static int create_buffer(DisplayBuffer *buffer)
{
    const SceSize bytes = 4 * DISPLAY_STRIDE * DISPLAY_HEIGHT;
    const SceSize allocation = ALIGN_UP(bytes, 256 * 1024);

    buffer->memblock = sceKernelAllocMemBlock(
        "FirstLightGXMBuffer", SCE_KERNEL_MEMBLOCK_TYPE_USER_CDRAM_RW,
        allocation, NULL);
    if (buffer->memblock < 0) return buffer->memblock;

    int result = sceKernelGetMemBlockBase(buffer->memblock, &buffer->data);
    if (result < 0) return result;

    result = sceGxmMapMemory(buffer->data, allocation, SCE_GXM_MEMORY_ATTRIB_RW);
    if (result < 0) return result;

    result = sceGxmColorSurfaceInit(
        &buffer->surface, SCE_GXM_COLOR_FORMAT_A8B8G8R8,
        SCE_GXM_COLOR_SURFACE_LINEAR, SCE_GXM_COLOR_SURFACE_SCALE_NONE,
        SCE_GXM_OUTPUT_REGISTER_SIZE_32BIT, DISPLAY_WIDTH, DISPLAY_HEIGHT,
        DISPLAY_STRIDE, buffer->data);
    if (result < 0) return result;

    return sceGxmSyncObjectCreate(&buffer->sync);
}

static int graphics_init(void)
{
    SceGxmInitializeParams params;
    memset(&params, 0, sizeof(params));
    params.displayQueueMaxPendingCount = MAX_PENDING_SWAPS;
    params.displayQueueCallback = display_callback;
    params.displayQueueCallbackDataSize = sizeof(void *);
    params.parameterBufferSize = SCE_GXM_DEFAULT_PARAMETER_BUFFER_SIZE;

    int result = sceGxmInitialize(&params);
    if (result < 0) return result;

    for (unsigned int i = 0; i < BUFFER_COUNT; ++i) {
        result = create_buffer(&buffers[i]);
        if (result < 0) return result;
        memset(buffers[i].data, 0, 4 * DISPLAY_STRIDE * DISPLAY_HEIGHT);
        sceKernelCpuDcacheWritebackRange(
            buffers[i].data, 4 * DISPLAY_STRIDE * DISPLAY_HEIGHT);
    }
    return 0;
}

static void draw_test_pattern(DisplayBuffer *buffer, unsigned int frame)
{
    uint32_t *pixels = (uint32_t *)buffer->data;
    for (int y = 0; y < DISPLAY_HEIGHT; ++y) {
        for (int x = 0; x < DISPLAY_STRIDE; ++x) {
            uint8_t r = (uint8_t)((x + frame * 2) & 255);
            uint8_t g = (uint8_t)((y + frame) & 255);
            uint8_t b = (uint8_t)(32 + ((x / 4 + y / 4 + frame) & 127));
            pixels[y * DISPLAY_STRIDE + x] =
                0xFF000000u | ((uint32_t)r << 16) |
                ((uint32_t)g << 8) | b;
        }
    }
    /* Make CPU writes visible before the display controller reads CDRAM. */
    sceKernelCpuDcacheWritebackRange(
        buffer->data, 4 * DISPLAY_STRIDE * DISPLAY_HEIGHT);
}

static void present_frame(void)
{
    sceGxmPadHeartbeat(&buffers[back_index].surface, buffers[back_index].sync);
    sceGxmDisplayQueueAddEntry(
        buffers[front_index].sync, buffers[back_index].sync,
        &buffers[back_index].data);
    front_index = back_index;
    back_index = (back_index + 1) % BUFFER_COUNT;
    sceDisplayWaitVblankStart();
}

static void graphics_shutdown(void)
{
    /* Wait for queued display work before releasing its backing memory. */
    sceGxmDisplayQueueFinish();
    for (unsigned int i = 0; i < BUFFER_COUNT; ++i) {
        if (buffers[i].sync) sceGxmSyncObjectDestroy(buffers[i].sync);
        if (buffers[i].data) sceGxmUnmapMemory(buffers[i].data);
        if (buffers[i].memblock >= 0) sceKernelFreeMemBlock(buffers[i].memblock);
    }
    sceGxmTerminate();
}

int main(void)
{
    for (unsigned int i = 0; i < BUFFER_COUNT; ++i) buffers[i].memblock = -1;
    if (graphics_init() < 0) sceKernelExitProcess(1);

    SceCtrlData pad;
    unsigned int frame = 0;
    for (;;) {
        sceCtrlPeekBufferPositive(0, &pad, 1);
        if (pad.buttons & SCE_CTRL_START) break;

        draw_test_pattern(&buffers[back_index], frame++);
        present_frame();
    }

    graphics_shutdown();
    sceKernelExitProcess(0);
    return 0;
}
