# Technical Decision Log

This log records decisions that affect the engine's foundations. Decisions can be revised when new measurements justify a change.

## D-001 — Start with a native Vita application

**Status:** Accepted  
**Decision:** Make the physical PS Vita the primary runtime target. Use Vita3K only for smoke tests and regression checks.

**Reason:** Emulator success does not prove real GPU timing, memory behaviour, input latency, thermal behaviour, or hardware compatibility.

**Validation:** Run the same build and benchmark on PCH-1000 and PCH-2000, recording separate results.

## D-002 — Use vitaGL to reach the first rotating mesh; keep direct GXM as a comparison

**Status:** Provisional  
**Decision:** The First Light prototype uses vitaGL, which translates a supported OpenGL subset to SceGxm. Do not describe this as a direct-SceGxm renderer.

**Reason:** It provides a shorter route to a native, hardware-accelerated 3D mesh and a measurable application loop. The current VitaSDK sample repository no longer includes its older standalone GXM sample set, while vitaGL maintains working graphics samples.

**Costs and risks:** vitaGL is a third-party dependency with GPL/LGPL licensing files and a runtime shader-compiler-module prerequisite. Its abstraction and compatibility costs must be measured, and distribution obligations reviewed before bundling it.

**What would change this decision:** A direct-GXM prototype that builds reproducibly, draws the same scene, has a maintainable shader/tool workflow, and demonstrates a meaningful improvement in CPU frame time, GPU frame time, memory use, startup time, or control over platform features.

**Fair comparison:** Same device, firmware, resolution, mesh, camera, colour/depth format, frame pacing, and test duration. Record CPU frame time, GPU time or a clearly labelled proxy, frame-time percentiles, memory pools, and visual differences.

## D-003 — Validate PCH-1000 and PCH-2000 independently

**Status:** Accepted  
**Decision:** Both hardware revisions are first-class test targets. Every benchmark result must identify its model.

**Reason:** They share the main CPU/GPU/memory architecture but differ in display technology and other hardware details. Independent runs guard against accidental assumptions about display, firmware, or device-specific behaviour.

**Validation:** Keep separate result rows and capture at least one successful launch/render/exit cycle on each model.

## D-004 — Treat native GXM as a first-class backend research path

**Status:** Accepted for investigation; performance advantage remains unproven by our own tests.

**Evidence:** The [Godot 4 PS Vita port by WolffsRoom](https://github.com/WolffsRoom/godot4-psvita) documents a native GXM 2D renderer and an experimental native GXM 3D renderer. Its README describes batching, GPU resource pools, CPU/GPU overlap and several real-hardware game validations, including a reported playable 3D port around 30 FPS.

**Interpretation:** This is strong evidence that substantial direct-GXM rendering is practical on Vita and that low-level control enables important optimizations. It is not a controlled head-to-head benchmark proving direct GXM is always faster than vitaGL. The upstream project's separate Deltarune release notes also describe its GXM backend as experimental and retain vitaGL as the recommended default because feature/performance parity is incomplete.

**Decision:** Continue building our own direct-GXM path. Study the public techniques, but implement our own renderer and verify licensing before incorporating any upstream code.

**Required validation:** Render the same First Light scene with both backends on the same physical Vita, with identical settings and repeated runs. Record CPU frame time, GPU time where measurable, frame-time distribution, memory, draw calls and visual parity. Until then, any performance advantage is a hypothesis, not a result.
