# Benchmark Scene 001 — First Light

**Purpose:** Establish the first repeatable native graphics baseline.

## Scene definition

- One original cube mesh.
- 8 geometric corners, 12 triangles.
- Six flat face colours.
- One perspective camera.
- One depth buffer.
- No textures, shadows, transparency, post-processing, particles, AI, or audio.
- Fixed 960 × 544 logical display resolution.
- Continuous rotation at a time-based angular velocity.
- On-screen FPS and last-frame-time display.

## Test procedure

1. Build from a clean checkout and record the commit SHA.
2. Install and launch on a physical Vita.
3. Allow 30 seconds for startup/settling.
4. Capture at least 120 seconds of continuous rendering.
5. Record average FPS, average frame time, and any available 1% low / 99th-percentile frame time.
6. Repeat three times on each device where possible.
7. Record whether the device is PCH-1000 or PCH-2000.
8. Do not compare results across different SDK/backend revisions without labelling the change.

## Controlled variables

- Same scene, code commit, build flags, resolution and test duration.
- Record firmware, homebrew environment, VitaSDK revision, vitaGL revision, power state and battery state.
- Do not enable overclock plugins for the baseline. If testing overclocking later, create a separate result row and label it.
- Vita3K results are separate and never count as hardware validation.

## Success criteria for M0

- Application launches and exits without a crash.
- Cube is visibly rendered and rotates continuously.
- FPS/frame-time overlay updates.
- One successful run is recorded on PCH-1000 and one on PCH-2000.
- Evidence is attached or linked for each hardware model.
