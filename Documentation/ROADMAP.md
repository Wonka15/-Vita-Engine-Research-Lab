# Roadmap

This roadmap is deliberately milestone-based. A milestone is complete only when its result builds, runs, and has reproducible evidence.

## M0 — Repository and toolchain

- [ ] Confirm repository structure and contribution conventions.
- [ ] Install a supported VitaSDK release.
- [ ] Build and run the official hello-world sample.
- [ ] Record host OS, SDK version, Vita firmware/homebrew environment, and build instructions.

## M1 — First graphics proof of concept

- [ ] Create a minimal Vita application.
- [ ] Initialize the display and graphics system.
- [ ] Clear the screen and present frames reliably.
- [ ] Draw a single indexed 3D mesh.
- [ ] Add a camera and basic input.
- [ ] Measure frame time and verify behavior on hardware or a clearly identified emulator.

## M2 — Renderer foundation

- [ ] Define vertex, index, mesh, texture, and material representations.
- [ ] Add transform and camera math.
- [ ] Add depth testing and basic texture sampling.
- [ ] Separate platform code from renderer code.
- [ ] Add a repeatable test scene.

## M3 — Asset pipeline and diagnostics

- [ ] Choose simple documented runtime asset formats.
- [ ] Add host-side validation/conversion tools.
- [ ] Track memory usage and resource lifetime.
- [ ] Add frame timing, draw-call, and allocation diagnostics.

## M4 — Visual feature experiments

- [ ] Compare simple lighting models.
- [ ] Test normal mapping and material variants.
- [ ] Evaluate fog, transparency, and environment rendering.
- [ ] Prototype shadow and reflection techniques only after baseline performance is known.
- [ ] Document visual quality and measured costs.

## M5 — Reusable engine

- [ ] Stabilize public engine interfaces.
- [ ] Add an example project that uses no Halo assets.
- [ ] Document how to add a renderer feature or platform service.
- [ ] Publish known limitations and benchmark methodology.

## Acceptance rule

No feature is called supported until it has a buildable implementation, a test or sample, documented limitations, and measured results where performance matters.


## M6 — Performance scaling and frame timing

- [ ] Add CPU timing scopes for simulation, scene preparation, and render submission.
- [ ] Add GPU timing or the best available platform-appropriate approximation.
- [ ] Record draw calls, triangles, render passes, active resolution, and memory use.
- [ ] Establish fixed-resolution baseline results on physical Vita hardware.
- [ ] Add independent controls for lights, shadows, particles, and object update frequency.
- [ ] Implement named quality profiles over those independent controls.
- [ ] Test dynamic resolution only after GPU bottleneck detection is reliable.
- [ ] Separate simulation timing from presentation timing.
- [ ] Evaluate background workers only where profiling demonstrates a benefit.

See [Vita Performance Scaling and Rendering Profiles](VITA_PERFORMANCE_SCALING.md) for the research basis and validation methodology.
