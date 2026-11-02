/*
 * Vita Performance Runtime (VPR) - portable C99 core.
 * Copyright (c) 2026 Vita Engine Research Lab contributors.
 * SPDX-License-Identifier: MIT
 */
#ifndef VPR_H
#define VPR_H
#include <stddef.h>
#include <stdint.h>
#ifdef __cplusplus
extern "C" {
#endif

#define VPR_VERSION_MAJOR 0
#define VPR_VERSION_MINOR 1
#define VPR_VERSION_PATCH 0

typedef enum VPR_Result {
    VPR_OK = 0,
    VPR_ERROR_INVALID_ARGUMENT = -1,
    VPR_ERROR_INVALID_STATE = -2,
    VPR_ERROR_CLOCK_MOVED_BACKWARDS = -3,
    VPR_ERROR_BUFFER_TOO_SMALL = -4,
    VPR_ERROR_WRITE_FAILED = -5
} VPR_Result;

/* Return a monotonically increasing timestamp in nanoseconds. */
typedef uint64_t (*VPR_ClockNowFn)(void *user_data);
/* Return 0 on success; non-zero stops CSV output. */
typedef int (*VPR_WriteFn)(void *user_data, const char *bytes, size_t byte_count);

typedef struct VPR_Config {
    VPR_ClockNowFn clock_now;
    void *clock_user_data;
} VPR_Config;

/* Caller owns sample_storage. A full ring overwrites its oldest sample. */
typedef struct VPR_Profiler {
    VPR_ClockNowFn clock_now;
    void *clock_user_data;
    uint64_t *sample_storage;
    size_t sample_capacity;
    size_t sample_count;
    size_t next_sample_index;
    uint64_t total_frames;
    uint64_t frame_start_ns;
    uint64_t last_frame_ns;
    uint64_t minimum_frame_ns;
    uint64_t maximum_frame_ns;
    int frame_is_open;
} VPR_Profiler;

typedef struct VPR_Stats {
    size_t sample_count;
    uint64_t total_frames;
    double average_frame_time_ms;
    double average_fps;
    double one_percent_low_fps;
    double p99_frame_time_ms;
    double minimum_frame_time_ms;
    double maximum_frame_time_ms;
} VPR_Stats;

VPR_Result VPR_ProfilerInit(VPR_Profiler *profiler, const VPR_Config *config,
                            uint64_t *sample_storage, size_t sample_capacity);
VPR_Result VPR_BeginFrame(VPR_Profiler *profiler);
VPR_Result VPR_EndFrame(VPR_Profiler *profiler);

/* scratch_storage must hold at least profiler->sample_count uint64_t values. */
VPR_Result VPR_GetStats(const VPR_Profiler *profiler, VPR_Stats *stats,
                        uint64_t *scratch_storage, size_t scratch_capacity);

/* Stream retained samples as CSV through the application's callback. */
VPR_Result VPR_WriteCSV(const VPR_Profiler *profiler, VPR_WriteFn write_fn,
                        void *write_user_data);
const char *VPR_GetVersionString(void);

#ifdef __cplusplus
}
#endif
#endif
