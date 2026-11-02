# Vita Performance Runtime

VPR 0.1.0 is a portable C99 profiling library for games that integrate it. It is not a background booster. The core uses a caller-supplied monotonic nanosecond clock and caller-owned sample memory. It records frame durations, computes average FPS, p99 frame time and an estimated 1% low, and streams samples to CSV through a callback. VitaSDK timer integration and physical Vita testing are still pending.
