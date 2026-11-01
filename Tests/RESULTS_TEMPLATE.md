# First Light — Results Template

Copy this table for every tested build. Keep Fat and Slim measurements separate.

| Date (UTC) | Commit | Device model | Firmware / homebrew | VitaSDK | Backend / revision | Resolution | Avg FPS | Avg frame (ms) | 1% low FPS / p99 ms | Main RAM used/free | VRAM used/free | Duration | Evidence / notes |
|---|---|---|---|---|---|---:|---:|---:|---:|---:|---:|---:|---|
| Not tested | — | PCH-1000 | — | — | — | 960×544 | — | — | — | — | — | — | Awaiting physical test |
| Not tested | — | PCH-2000 | — | — | — | 960×544 | — | — | — | — | — | — | Awaiting physical test |

## Measurement notes

- The current on-screen overlay reports instantaneous FPS (updated over a one-second window) and the most recent frame interval.
- The overlay does not yet report allocator-level main RAM or VRAM use. Fill those fields only when measured with a documented method; otherwise leave them blank.
- Do not present FPS as a substitute for frame-time distribution. A stable 30 FPS target corresponds to approximately 33.3 ms per frame; 60 FPS corresponds to approximately 16.7 ms.
- Record whether the result is **build-tested**, **emulator-tested**, **hardware-tested PCH-1000**, or **hardware-tested PCH-2000**.
