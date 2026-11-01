# Vita Performance Scaling and Rendering Profiles

**Status:** Research reference and proposed engine-design guidance  
**Platform:** PlayStation Vita  
**Primary case study:** Halo: Combat Evolved — PS Vita port  
**Scope:** Performance architecture, quality scaling, frame pacing, and hardware validation

## Executive summary

A handheld renderer should not treat performance as a single graphics-quality slider. The Vita port's settings demonstrate a more useful approach: adjust GPU-heavy work when the GPU is the bottleneck, adjust simulation and update frequency when the CPU is the bottleneck, and keep presentation timing separate from simulation timing.

For Vita Engine Research Lab, the lesson is architectural. Build measurable systems with independent quality controls, explicit budgets, and fallbacks. Do not copy game-specific code or assume that a setting which helps one scene will help every scene.

## 1. Performance is a CPU/GPU workload problem

### GPU-bound scenes

A scene is GPU-bound when graphics work is the limiting factor. Potential controls include:

- Lowering render resolution.
- Reducing expensive lighting and shadow passes.
- Reducing particle density, screen-space effects, and overdraw.
- Simplifying materials or reducing shader complexity.
- Reducing the number and size of render targets.

Dynamic resolution is useful only when GPU load is the limiting factor. Lowering resolution does not remove CPU work such as AI, animation updates, visibility traversal, draw-call preparation, or sound processing.

### CPU-bound scenes

A scene is CPU-bound when simulation, scene preparation, or command generation limits frame time. Potential controls include:

- Updating distant or low-priority objects less frequently.
- Reducing AI decision frequency where gameplay permits.
- Simplifying visibility and scene traversal.
- Batching work to reduce submission overhead.
- Moving independent loading, decompression, audio, and logging tasks to background workers.
- Reducing expensive per-object calculations.

A CPU-bound scene may show little or no improvement when render resolution is lowered. The Halo CE Vita port's beach-landing example is a useful case study: a crowded scene can be limited by character and simulation work rather than pixel throughput.

## 2. Quality profiles as coordinated presets

The port groups related settings into Performance, Balanced, and Quality profiles. Manually changing an individual profile-controlled setting switches the configuration to Custom. This is a good usability pattern: profiles provide sensible starting points, while advanced users retain control.

The table below records the settings described in the port's graphics documentation. These are reference values from that port, not recommended defaults for our engine.

| Setting | Performance | Balanced (default) | Quality |
|---|---|---|---|
| Render resolution | Dynamic | Dynamic | Dynamic |
| Dynamic minimum | 50% | 63% | 75% |
| Model detail | Low | Medium | High |
| Hide distant objects | Small | Small | Tiny |
| Object shadows | Off | Off | Near only |
| Dynamic lights | 4 | 4 | 8 |
| Effects quality | Performance | Full | Full |
| Sun rays | Off | Off | On |
| Particles | Reduced | Reduced | Reduced |
| Particle density | Half | Full | Full |
| Scenery updates | Quarter | Quarter | Half |
| Object lighting | Third | Third | Half |
| AI think rate | Adaptive | Adaptive | Adaptive |
| Sound occlusion | Every 6th update | Every 3rd update | Every 3rd update |
| Sound updates | Every 2nd frame | Every 2nd frame | Every frame |

### Interpreting the profiles

- **Performance** prioritizes demanding fights and headroom for stable presentation.
- **Balanced** improves visual richness without enabling the two costly extra scene families identified by the port: object-shadow rendering and sun rays.
- **Quality** is intended for quieter scenes and cinematics, where the additional visual work is more likely to remain playable.

The port's documentation notes that Balanced adds reflections, shine, grass, denser particle presentation, improved models, and a sharper image without adding the extra GPU passes used by object shadows and sun rays. Quality enables near object shadows and sun rays. It can also increase CPU cost in large fights.

The port keeps particles reduced in all three presets while varying particle density. These are separate controls: one describes the particle quality mode, while the other changes how densely particles are presented. Do not collapse them into one setting in our own UI.

### Proposed profile model for our engine

We should implement profiles as named collections of independently stored settings, not hard-coded renderer modes. Each setting should remain inspectable and overridable.

| Engine control | Performance | Balanced | Quality |
|---|---|---|---|
| Resolution scale | Lower minimum | Medium minimum | Higher minimum |
| Mesh detail | Reduced | Standard | High |
| Shadow work | Disabled | Disabled or very limited | Selective |
| Dynamic lights | Strict budget | Moderate budget | Expanded budget |
| Particles | Reduced density | Standard density | Higher density |
| Object update rate | Lower for distant objects | Adaptive | More frequent |
| Post-processing | Minimal | Selected effects | Expanded, if measured viable |

These are design placeholders. Actual values must come from repeatable tests on a physical Vita.

## 3. Dynamic resolution and render scaling

The reference port's Dynamic mode lowers resolution only when the GPU is the limiting factor, down to a profile-specific minimum. It can also select a lower maximum when a map's video-memory requirements prevent a full-resolution target from fitting.

For our renderer:

1. Define a supported range of render scales.
2. Measure GPU frame time over a rolling window.
3. Change scale gradually to avoid visible oscillation.
4. Never lower resolution in response to a CPU-only bottleneck.
5. Record the active scale in performance logs.
6. Re-evaluate the maximum scale when scene or render-target requirements change.

A fixed scale should remain available for controlled benchmarks. Dynamic scaling is an optimization policy, not a substitute for understanding the bottleneck.

## 4. Expensive graphics features and render passes

The port identifies object shadows and sun rays as costly because they require additional scene work for the graphics chip each frame. This illustrates a broader rule: a feature's cost is not measured only by its visual complexity. Additional passes can repeat geometry submission, state changes, render-target work, and fragment processing.

Our renderer should:

- Keep a baseline path with opaque geometry, depth testing, and simple lighting.
- Add optional passes only when their visual benefit justifies their measured cost.
- Expose shadow distance, light count, and effect quality as explicit budgets.
- Avoid enabling every effect globally by default.
- Measure transparent surfaces and overdraw rather than assuming they can simply be removed.

Transparency needs particular care. The reference port retains see-through surfaces such as Covenant field-generator domes and visors in all profiles. This is a useful reminder that visual importance and technical cost must be balanced deliberately; transparency is not automatically disposable.

## 5. Simulation rate and presentation rate

The port separates its 30 Hz simulation tick from display presentation. Its optional frame-interpolation mode can present intermediate blended frames, targeting up to 60 displayed frames per second while the game keeps up. When it cannot maintain that cadence, it falls back to presenting one frame per simulation tick.

This is not the same as running gameplay simulation at 60 Hz.

### Proposed timing architecture

- **Simulation clock:** advances gameplay at a fixed, documented tick rate.
- **Render clock:** can run independently and sample the latest simulation state.
- **Interpolation:** blends between previous and current simulation states when valid.
- **Fallback:** disables or reduces interpolation when timing, latency, or performance makes it unsuitable.

For our first engine prototype, prioritize stable frame pacing and clear timing instrumentation. Treat interpolation as a later experiment; it can introduce latency or visual artifacts if implemented without careful state handling.

## 6. Background work and multicore design

The Vita port documents helper work moved to the normally reserved fourth CPU core with CapUnlocker support. Examples include audio, display-queue work, loading, map decompression, checkpoint writing, shader compilation, and logging. Its game, render, and tick threads remain on their original cores.

The architectural lesson is to isolate independent work behind job or task interfaces. Do not assume that every task belongs on a helper core, and do not move work merely to make a thread diagram look sophisticated.

### Proposed worker categories

- Asset I/O and decompression.
- Shader or pipeline preparation.
- Audio processing.
- Logging and telemetry.
- Background resource cleanup where safe.

Keep dependencies explicit. GPU resource creation, display submission, and other thread-sensitive operations must follow the platform API's synchronization rules. Profile the main render and simulation threads before adding concurrency.

## 7. Profiling and benchmark methodology

Every performance claim should identify the exact build, device, scene, profile, and test conditions.

### Required measurements

- Average, median, and high-percentile frame time.
- CPU time by major subsystem.
- GPU time where available.
- Render resolution and active dynamic scale.
- Draw calls, triangles, state changes, and render passes.
- Memory use and peak allocation.
- Number of visible objects, lights, particles, and animated actors.
- Frame pacing, missed deadlines, and thermal or power conditions.

### Repeatable benchmark scenes

1. **Open environment:** large visible area, distant geometry, and long visibility ranges.
2. **Effects-heavy scene:** particles, alpha blending, and overlapping transparent surfaces.
3. **Simulation-heavy scene:** many animated characters, AI agents, and scripted interactions.

Run each scene at fixed settings first. Then change one variable at a time. Keep browser tests, Vita3K results, and physical Vita measurements in separate result categories.

### Evidence labels

- **Documented:** stated in an upstream document.
- **Source-confirmed:** verified in the relevant source revision.
- **Emulator-tested:** reproduced in Vita3K.
- **Hardware-tested:** measured on a physical PS Vita.
- **Hypothesis:** plausible, but not yet verified.

## 8. What this means for Vita Engine Research Lab

Our implementation should be built in this order:

1. Add CPU and GPU timing instrumentation to the smallest working renderer.
2. Establish a fixed-resolution baseline.
3. Add scene and render-pass counters.
4. Create independent quality controls for resolution, lights, shadows, particles, and object updates.
5. Add named profiles that only configure those controls.
6. Implement adaptive resolution only after GPU bottlenecks can be detected reliably.
7. Separate simulation and presentation timing.
8. Evaluate background jobs after profiling identifies safe, worthwhile tasks.
9. Validate every optimization on physical hardware.

## Conclusion

The strongest lesson from the Halo CE Vita port is not a particular graphics preset. It is the discipline of treating performance as a collection of measurable workloads. GPU limits call for changes to rendering cost; CPU limits call for changes to simulation, scene preparation, and update frequency. Frame presentation can be decoupled from simulation, and independent background work can be scheduled without entangling the engine's foundations.

Vita Engine Research Lab should adopt these principles while building its own systems. We should begin with a reliable baseline, expose clear quality controls, profile before optimizing, and keep every performance claim tied to reproducible evidence from the actual hardware.

## Primary research reference

- [Halo: Combat Evolved — PS Vita port repository](https://github.com/BirchWoodGod/halo-ce-vita)
- [Vita port README and graphics settings](https://github.com/BirchWoodGod/halo-ce-vita/blob/main/port/vita/README.md)
- [Project roadmap and release history](https://github.com/BirchWoodGod/halo-ce-vita/blob/main/ROADMAP.md)

This document describes engineering observations and proposed original design work. It does not reproduce proprietary Halo assets or source code.
