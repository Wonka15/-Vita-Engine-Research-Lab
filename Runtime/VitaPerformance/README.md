# Vita Performance Runtime (VPR)

**Version:** 0.1.0 — portable C99 core.

VPR is a universal, opt-in profiling library for Vita games and homebrew. It must be integrated into an application; it is not a system-wide booster.

## Implemented

- Caller-owned fixed-capacity sample ring; no heap allocation.
- Caller-supplied monotonic nanosecond clock.
- Frame begin/end timing.
- Average FPS and frame time over the retained sample window.
- p99 frame time and a derived 1% low estimate.
- Minimum and maximum frame time.
- CSV export through an application callback.
- Deterministic host tests using a fake clock.

## Build and test

Requires CMake 3.16+ and a C99 compiler.

~~~sh
cmake -S Runtime/VitaPerformance -B build/vpr
cmake --build build/vpr --parallel
ctest --test-dir build/vpr --output-on-failure
~~~

## Integration

Add src/vpr_profiler.c to the game build and include include/vpr.h. Provide a monotonic clock callback that returns nanoseconds. Allocate the profiler, sample ring and percentile scratch array in application-owned memory. Call VPR_BeginFrame immediately before the workload and VPR_EndFrame immediately after it.

Call VPR_GetStats outside the hot frame path. VPR_WriteCSV sends retained frame samples to a callback supplied by the application, leaving file-system handling to the game.

## Important limitations

- VPR 0.1 measures CPU-side elapsed time around the application's chosen frame boundary. It does not measure asynchronous GPU execution time.
- The 1% low is calculated as the reciprocal of the nearest-rank p99 frame time; it is not the mean of the slowest one percent.
- Statistics describe the most recent samples retained in the ring. The total frame counter includes all completed frames since initialization.
- One thread should own a profiler instance.
- A VitaSDK timer adapter, Vita sample integration, physical-console tests, memory diagnostics, NEON routines, culling and LOD are future work.

See [VPR design notes](../../Documentation/VPR_DESIGN.md) for the architecture and validation rules.
