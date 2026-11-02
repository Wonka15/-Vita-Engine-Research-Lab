#include "vpr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define VPR_NS_PER_SECOND 1000000000.0
#define VPR_NS_PER_MS 1000000.0

static int compare_u64(const void *a, const void *b)
{
    const uint64_t left = *(const uint64_t *)a;
    const uint64_t right = *(const uint64_t *)b;
    return (left > right) - (left < right);
}

static uint64_t sample_at(const VPR_Profiler *p, size_t logical_index)
{
    const size_t oldest = (p->next_sample_index + p->sample_capacity
        - p->sample_count) % p->sample_capacity;
    return p->sample_storage[(oldest + logical_index) % p->sample_capacity];
}

VPR_Result VPR_ProfilerInit(VPR_Profiler *p, const VPR_Config *config,
                            uint64_t *storage, size_t capacity)
{
    if (!p || !config || !config->clock_now || !storage || capacity == 0)
        return VPR_ERROR_INVALID_ARGUMENT;
    memset(p, 0, sizeof(*p));
    p->clock_now = config->clock_now;
    p->clock_user_data = config->clock_user_data;
    p->sample_storage = storage;
    p->sample_capacity = capacity;
    return VPR_OK;
}

VPR_Result VPR_BeginFrame(VPR_Profiler *p)
{
    if (!p || !p->clock_now || !p->sample_storage || p->sample_capacity == 0)
        return VPR_ERROR_INVALID_ARGUMENT;
    if (p->frame_is_open) return VPR_ERROR_INVALID_STATE;
    p->frame_start_ns = p->clock_now(p->clock_user_data);
    p->frame_is_open = 1;
    return VPR_OK;
}

VPR_Result VPR_EndFrame(VPR_Profiler *p)
{
    uint64_t end_ns, duration_ns;
    if (!p || !p->clock_now) return VPR_ERROR_INVALID_ARGUMENT;
    if (!p->frame_is_open) return VPR_ERROR_INVALID_STATE;
    end_ns = p->clock_now(p->clock_user_data);
    p->frame_is_open = 0;
    if (end_ns < p->frame_start_ns) return VPR_ERROR_CLOCK_MOVED_BACKWARDS;

    duration_ns = end_ns - p->frame_start_ns;
    p->sample_storage[p->next_sample_index] = duration_ns;
    p->next_sample_index = (p->next_sample_index + 1) % p->sample_capacity;
    if (p->sample_count < p->sample_capacity) p->sample_count++;
    p->total_frames++;
    p->last_frame_ns = duration_ns;
    if (p->total_frames == 1 || duration_ns < p->minimum_frame_ns)
        p->minimum_frame_ns = duration_ns;
    if (p->total_frames == 1 || duration_ns > p->maximum_frame_ns)
        p->maximum_frame_ns = duration_ns;
    return VPR_OK;
}

VPR_Result VPR_GetStats(const VPR_Profiler *p, VPR_Stats *stats,
                        uint64_t *scratch, size_t scratch_capacity)
{
    size_t i, p99_index;
    uint64_t total_ns = 0;
    if (!p || !stats || !p->sample_storage) return VPR_ERROR_INVALID_ARGUMENT;
    if (p->sample_count == 0) return VPR_ERROR_INVALID_STATE;
    if (!scratch || scratch_capacity < p->sample_count)
        return VPR_ERROR_BUFFER_TOO_SMALL;

    for (i = 0; i < p->sample_count; ++i) {
        scratch[i] = sample_at(p, i);
        total_ns += scratch[i];
    }
    qsort(scratch, p->sample_count, sizeof(scratch[0]), compare_u64);
    memset(stats, 0, sizeof(*stats));
    stats->sample_count = p->sample_count;
    stats->total_frames = p->total_frames;
    stats->average_frame_time_ms =
        ((double)total_ns / (double)p->sample_count) / VPR_NS_PER_MS;
    if (total_ns > 0)
        stats->average_fps = ((double)p->sample_count * VPR_NS_PER_SECOND)
            / (double)total_ns;

    /* Nearest-rank p99; 1% low is the reciprocal of that frame time. */
    p99_index = (p->sample_count * 99 + 99) / 100;
    if (p99_index > 0) p99_index--;
    if (p99_index >= p->sample_count) p99_index = p->sample_count - 1;
    stats->p99_frame_time_ms = (double)scratch[p99_index] / VPR_NS_PER_MS;
    if (scratch[p99_index] > 0)
        stats->one_percent_low_fps = VPR_NS_PER_SECOND / (double)scratch[p99_index];
    stats->minimum_frame_time_ms = (double)scratch[0] / VPR_NS_PER_MS;
    stats->maximum_frame_time_ms =
        (double)scratch[p->sample_count - 1] / VPR_NS_PER_MS;
    return VPR_OK;
}

VPR_Result VPR_WriteCSV(const VPR_Profiler *p, VPR_WriteFn writer, void *user_data)
{
    char line[128];
    size_t i;
    int length;
    if (!p || !writer || p->sample_count == 0) return VPR_ERROR_INVALID_ARGUMENT;
    length = snprintf(line, sizeof(line),
        "frame_index,frame_time_ns,frame_time_ms,instantaneous_fps\n");
    if (length < 0 || (size_t)length >= sizeof(line)
        || writer(user_data, line, (size_t)length) != 0)
        return VPR_ERROR_WRITE_FAILED;

    for (i = 0; i < p->sample_count; ++i) {
        const uint64_t ns = sample_at(p, i);
        const double ms = (double)ns / VPR_NS_PER_MS;
        const double fps = ns ? VPR_NS_PER_SECOND / (double)ns : 0.0;
        length = snprintf(line, sizeof(line), "%lu,%llu,%.6f,%.3f\n",
            (unsigned long)(i + 1), (unsigned long long)ns, ms, fps);
        if (length < 0 || (size_t)length >= sizeof(line)
            || writer(user_data, line, (size_t)length) != 0)
            return VPR_ERROR_WRITE_FAILED;
    }
    return VPR_OK;
}

const char *VPR_GetVersionString(void)
{
    return "0.1.0";
}
