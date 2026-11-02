# Vita Performance Runtime — Design Notes

## Mission

Vita Performance Runtime (VPR) is a universal, opt-in native library that game and homebrew developers can integrate into their own applications. It is not a system-wide background booster and cannot optimize a closed-source game that does not integrate it.

## Language and portability

- Core implementation: ISO C99.
- Vita-specific adapters: C using VitaSDK APIs.
- Optional future convenience wrappers: C++ without making the core depend on C++.
- Optional CPU-specific paths: ARM NEON intrinsics, guarded by build-time platform checks and validated against portable C implementations.

The core must not include VitaSDK headers or assume a particular renderer. Platform services are supplied through narrow callbacks or optional adapters.

## Current implementation: 0.1.0

The first implementation is a deterministic frame profiler:
- Caller-owned fixed-size sample ring; no heap allocation.
- Platform-supplied monotonic nanosecond clock.
- Average FPS and frame time over the retained sample window.
- Nearest-rank p99 frame time and derived 1% low estimate.
- Min/max frame time.
- CSV streaming callback.
- Host tests driven by a fake clock.

The profiler measures CPU-side elapsed time around the caller's chosen frame boundary. It does not claim to measure asynchronous GPU execution time.

## Planned modules

1. **Core profiler:** frame timing, percentiles, benchmark sessions and report metadata.
2. **Platform adapters:** Vita monotonic timer, file output, device/build metadata.
3. **Memory diagnostics:** tagged allocation tracking and pool statistics, without forcing an allocator on applications.
4. **Math:** portable vector/matrix routines with optional NEON implementations and equivalence tests.
5. **Work scheduling guidance:** optional helpers only after real workloads demonstrate benefit; never take over an application's threads unexpectedly.
6. **Visibility/LOD helpers:** renderer-independent bounding-volume and distance tests, with integration examples.
7. **Graphics instrumentation:** backend-specific counters and GPU timing where supported; never label CPU proxies as GPU measurements.
8. **Adaptive quality recommendations:** opt-in recommendations first; application-controlled changes only.

## Benchmark rules

- Compare optimized and baseline paths using the same scene, camera, assets, resolution, build mode and duration.
- Separate warm-up from measurement.
- Retain raw frame samples as well as summary statistics.
- Publish device revision, firmware, SDK/toolchain, renderer, build identifier and emulator/physical-device status.
- Repeat runs and report variability.
- Never claim performance gains based solely on desktop tests or Vita3K.

## Definition of done for a module

A module is not complete merely because it compiles. It needs:
- A documented public API.
- Portable fallback where practical.
- Tests for correctness and edge cases.
- A repeatable benchmark if performance is the reason for the module.
- Explicit limitations and ownership/threading rules.
- VitaSDK build evidence and physical-device evidence when the feature depends on Vita hardware.
