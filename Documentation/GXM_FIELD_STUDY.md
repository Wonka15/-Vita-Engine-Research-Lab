# Field Study: Godot 4 for PS Vita — Native GXM

**Project:** [WolffsRoom/godot4-psvita](https://github.com/WolffsRoom/godot4-psvita)  
**Evidence class:** Public source and project documentation; reported physical-hardware validation by the project author  
**Research relevance:** High — a substantial independent example of direct SceGxm rendering in a modern engine

## Why this matters

This project demonstrates that native GXM can support much more than a minimal clear-screen demo. It integrates a custom Godot 4.8-dev7 runtime, a Vita OS layer, native GXM 2D rendering, experimental GXM 3D rendering, and a Windows editor that exports Vita VPKs.

The project documentation describes a native GXM 2D renderer that translates Godot CanvasItem commands into GXM draws, and an experimental 3D renderer that submits mesh geometry through GXM. It reports real-hardware validation across several 2D games and reports OVERBURDEN, a 3D port, playable on hardware at approximately 30 FPS. These are project-reported results; our lab has not independently reproduced them.

## Techniques worth studying

- **Native API ownership:** direct SceGxm renderer code rather than relying on a general OpenGL wrapper for the rendering path.
- **Draw batching:** merge consecutive compatible quads and avoid redundant clip-state changes.
- **GPU resource management:** explicit texture budgets, vertex pools, and GPU-memory arenas.
- **CPU/GPU overlap:** prepare the next frame while the GPU processes the current frame; defer resource retirement until GPU work is complete.
- **Workload control:** triangle budgets, distance/screen-size culling and hysteresis.
- **Platform integration:** separate Vita loader, runtime, OS services and Windows export tooling.

These techniques are valuable architectural evidence. They are not a guarantee that every direct-GXM renderer will outperform every vitaGL workload.

## What the evidence does and does not establish

**Supported by the upstream project's documentation:**
- A native GXM 2D renderer has been implemented and used in named games on physical Vita hardware.
- A native GXM 3D renderer exists, but is explicitly experimental.
- The OVERBURDEN port is reported playable at around 30 FPS on hardware.
- The project includes batching, resource-pool and CPU/GPU-overlap work.

**Not established by this evidence alone:**
- That direct GXM is universally faster than vitaGL.
- That the same game, scene and settings were benchmarked head-to-head against vitaGL.
- That the experimental 3D renderer has feature or performance parity with mature renderers.
- That our First Light GXM prototype has been tested on a physical Vita.

The upstream project's Deltarune work is also instructive: its native GXM backend is described as experimental, with incomplete visual, shader, video, performance and chapter parity; the VitaGL backend remains the recommended default in that project's documented release notes. That is a useful reminder that direct control can improve specific bottlenecks or stability while increasing implementation work and compatibility risks.

## Implications for Vita Engine Research Lab

1. Keep direct GXM as a first-class backend research path, not merely an emergency fallback.
2. Study batching, GPU resource lifetime, texture formats, command submission and CPU/GPU overlap.
3. Do not copy code or assets without checking the exact license and dependency obligations. Prefer independently implemented designs based on public concepts.
4. Move beyond the current GXM display bring-up to a shader-backed cube using `sceGxmDraw`.
5. Run the same benchmark scene through vitaGL and direct GXM on the same physical Vita, using identical resolution, camera, mesh, frame pacing and duration.
6. Record CPU frame time, GPU time where genuinely measurable, frame-time percentiles, memory use, draw calls and visual differences.
7. Keep emulator results separate from physical hardware results.

## Research conclusion

Godot 4 PS Vita is strong evidence that a serious native GXM renderer is feasible and that techniques such as batching, explicit resource management and CPU/GPU overlap can make a meaningful practical difference. It strengthens the case for investing in our own direct-GXM backend. It does **not** settle the performance comparison against vitaGL; our controlled benchmark must do that.

## Sources

- [WolffsRoom/godot4-psvita — source and technical README](https://github.com/WolffsRoom/godot4-psvita)
- [WolffsRoom's Godot 4 Vita release discussion](https://www.reddit.com/r/VitaPiracy/comments/1xg94cz/release_godot_engine_4x_for_vita_v01/)
- [WolffsRoom Deltarune Vita — native GXM backend and limitations](https://github.com/WolffsRoom/DeltaruneVita/releases)
