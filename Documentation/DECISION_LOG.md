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


## D-005 — Evaluate VkVita as a Vulkan-style abstraction candidate

**Status:** Accepted for research only; not adopted as a dependency.

**Evidence:** [DrDecki/vkvita](https://github.com/DrDecki/vkvita) describes a static library implementing a large subset of Vulkan 1.0-style `vk*` calls and mapping them to SceGxm. Its README documents SPIR-V-to-Cg translation, shader tooling, samples, profiling and configurable runtime behaviour. It also documents important limits, including no compute support, one colour attachment per subpass and a fixed 960×544 swapchain.

**Reason:** A Vulkan-style interface may help us share renderer code across Vita and desktop platforms. It offers a potential middle ground between vitaGL and a fully direct SceGxm renderer.

**Risks:** It is experimental and has no formal releases; its `main` branch is described as the current build. Runtime shader compilation has vitaShaRK / `libshacccg.suprx` prerequisites unless precompiled GXP shaders are used. VkVita is GPL-3.0-or-later, so linking or distribution needs a deliberate license review. API coverage and performance are not yet independently verified by this project.

**Decision:** Add it to the research library and roadmap as a candidate, but do not vendor, link or make it the default backend yet. Keep our engine interfaces backend-neutral.

**Required validation:** Pin an upstream commit, build its samples, port the same First Light scene and benchmark it against vitaGL and direct GXM on the same physical Vita. Measure steady-state CPU frame time, FPS distribution, memory, shader startup/cache behaviour and feature gaps. A Vulkan-shaped API does not by itself imply Vulkan hardware support or better performance.
