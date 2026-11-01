# Performance and rendering tests

All optimization claims should be backed by repeatable tests.

## Baseline scene

The first benchmark scene should contain:

- One rotating indexed mesh.
- A fixed camera and deterministic animation.
- A documented vertex and index count.
- A documented texture size and format, when textures are added.
- A frame-time/FPS display or log.

## Record for every run

| Field | Value |
|---|---|
| Device | Vita model / emulator name and version |
| SDK | Exact VitaSDK release |
| Build | Debug or release |
| Resolution | Actual render resolution |
| Scene | Test scene identifier |
| Geometry | Vertex and triangle counts |
| Textures | Dimensions, formats, and total estimated bytes |
| Rendering | Backend and enabled features |
| Performance | Average and worst frame time; FPS if stable |
| Notes | Visual issues, stutter, crashes, or thermal/session conditions |

## Benchmark rules

- Change one major variable at a time.
- Run the same scene and camera path for comparisons.
- Separate emulator measurements from physical hardware.
- Report average and worst frame time rather than FPS alone.
- Do not claim a feature is fast based on a single short run.
