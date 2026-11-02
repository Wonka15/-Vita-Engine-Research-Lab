#include "vpr.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

typedef struct FakeClock { uint64_t now_ns; } FakeClock;
typedef struct CSVBuffer { char bytes[2048]; size_t used; } CSVBuffer;

static uint64_t fake_clock_now(void *user_data)
{
    return ((const FakeClock *)user_data)->now_ns;
}

static int append_csv(void *user_data, const char *bytes, size_t count)
{
    CSVBuffer *buffer = (CSVBuffer *)user_data;
    if (buffer->used + count >= sizeof(buffer->bytes)) return 1;
    memcpy(buffer->bytes + buffer->used, bytes, count);
    buffer->used += count;
    buffer->bytes[buffer->used] = '\0';
    return 0;
}

static void test_frame_timing_and_statistics(void)
{
    FakeClock clock = { 1000000000ULL };
    VPR_Config config = { fake_clock_now, &clock };
    VPR_Profiler profiler;
    uint64_t samples[4], scratch[4];
    VPR_Stats stats;

    assert(VPR_ProfilerInit(&profiler, &config, samples, 4) == VPR_OK);
    /* Exact frame durations: 10, 20, 30 and 40 milliseconds. */
    assert(VPR_BeginFrame(&profiler) == VPR_OK);
    clock.now_ns += 10000000ULL;
    assert(VPR_EndFrame(&profiler) == VPR_OK);
    assert(VPR_BeginFrame(&profiler) == VPR_OK);
    clock.now_ns += 20000000ULL;
    assert(VPR_EndFrame(&profiler) == VPR_OK);
    assert(VPR_BeginFrame(&profiler) == VPR_OK);
    clock.now_ns += 30000000ULL;
    assert(VPR_EndFrame(&profiler) == VPR_OK);
    assert(VPR_BeginFrame(&profiler) == VPR_OK);
    clock.now_ns += 40000000ULL;
    assert(VPR_EndFrame(&profiler) == VPR_OK);

    assert(VPR_GetStats(&profiler, &stats, scratch, 4) == VPR_OK);
    assert(stats.sample_count == 4 && stats.total_frames == 4);
    assert(stats.average_frame_time_ms > 24.99 && stats.average_frame_time_ms < 25.01);
    assert(stats.average_fps > 39.99 && stats.average_fps < 40.01);
    assert(stats.minimum_frame_time_ms > 9.99);
    assert(stats.maximum_frame_time_ms < 40.01);
    assert(stats.one_percent_low_fps > 24.99 && stats.one_percent_low_fps < 25.01);
}

static void test_ring_buffer_keeps_recent_samples(void)
{
    FakeClock clock = { 0 };
    VPR_Config config = { fake_clock_now, &clock };
    VPR_Profiler profiler;
    uint64_t samples[3], scratch[3];
    VPR_Stats stats;
    const uint64_t durations[] = { 1000000ULL, 2000000ULL, 3000000ULL, 4000000ULL };
    size_t i;

    assert(VPR_ProfilerInit(&profiler, &config, samples, 3) == VPR_OK);
    for (i = 0; i < 4; ++i) {
        assert(VPR_BeginFrame(&profiler) == VPR_OK);
        clock.now_ns += durations[i];
        assert(VPR_EndFrame(&profiler) == VPR_OK);
    }
    assert(VPR_GetStats(&profiler, &stats, scratch, 3) == VPR_OK);
    assert(stats.total_frames == 4 && stats.sample_count == 3);
    assert(stats.minimum_frame_time_ms > 1.99);
    assert(stats.maximum_frame_time_ms > 3.99);
}

static void test_state_and_csv(void)
{
    FakeClock clock = { 0 };
    VPR_Config config = { fake_clock_now, &clock };
    VPR_Profiler profiler;
    uint64_t samples[2];
    CSVBuffer csv = { { 0 }, 0 };

    assert(VPR_ProfilerInit(&profiler, &config, samples, 2) == VPR_OK);
    assert(VPR_EndFrame(&profiler) == VPR_ERROR_INVALID_STATE);
    assert(VPR_BeginFrame(&profiler) == VPR_OK);
    assert(VPR_BeginFrame(&profiler) == VPR_ERROR_INVALID_STATE);
    clock.now_ns += 16666667ULL;
    assert(VPR_EndFrame(&profiler) == VPR_OK);
    assert(VPR_WriteCSV(&profiler, append_csv, &csv) == VPR_OK);
    assert(strstr(csv.bytes, "frame_index,frame_time_ns") != NULL);
    assert(strstr(csv.bytes, "16666667") != NULL);
    assert(strcmp(VPR_GetVersionString(), "0.1.0") == 0);
}

int main(void)
{
    test_frame_timing_and_statistics();
    test_ring_buffer_keeps_recent_samples();
    test_state_and_csv();
    puts("VPR profiler tests passed.");
    return 0;
}
