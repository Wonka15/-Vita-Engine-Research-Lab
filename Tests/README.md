# Performance and rendering tests

The first test is deliberately tiny. It exists to validate the native application/render/present loop before adding engine systems.

## Scene 001 — First Light

See [the full scene definition](SCENES/FIRST_LIGHT.md) and [results template](RESULTS_TEMPLATE.md).

- One original cube mesh: 8 geometric corners, 12 triangles.
- Six flat face colours, one perspective camera and a depth buffer.
- Fixed 960 × 544 logical display.
- No textures, shadows, transparency, post-processing, particles, AI or audio.
- Time-based rotation and an on-screen FPS / last-frame-time display.

## Required device matrix

| Device | Model | Status |
|---|---|---|
| Original Vita / Fat | PCH-1000 | Not yet hardware-tested |
| Vita Slim | PCH-2000 | Not yet hardware-tested |

Never combine these rows into one unlabeled result.

## Record for every run

| Field | Value |
|---|---|
| Device | Exact model: PCH-1000 or PCH-2000 |
| Firmware / homebrew | Exact versions |
| SDK | VitaSDK release |
| Backend | vitaGL or direct GXM + revision |
| Build | Commit SHA and build flags |
| Resolution | Actual render resolution |
| Scene | Test scene identifier |
| Geometry | Vertex and triangle counts |
| Performance | Average FPS, average frame time, p99 or 1% low if available |
| Memory | Main RAM and VRAM, with measurement method |
| Duration | At least 120 seconds after a 30-second settle period |
| Evidence | Photo/video/log link and notes |

## Benchmark rules

- Change one major variable at a time.
- Run the same scene and camera path for comparisons.
- Keep emulator measurements separate from physical hardware.
- Report frame time as well as FPS.
- Do not claim a feature is fast based on one short run.
- Do not claim hardware validation without naming the tested console revision.
