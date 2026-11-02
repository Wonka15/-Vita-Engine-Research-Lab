# Roadmap

A milestone is complete only when its acceptance evidence exists. A source file alone does not mean the feature has been built or tested.

## M0 — First Light: native Vita graphics proof

- [x] Add a minimal native Vita application source prototype.
- [x] Add a rotating mesh and on-screen FPS/frame-time instrumentation source.
- [x] Define one repeatable baseline scene and results template.
- [ ] Build the VPK with the documented VitaSDK/vitaGL toolchain.
- [ ] Launch and verify clean exit on a physical PCH-1000.
- [ ] Launch and verify clean exit on a physical PCH-2000.
- [ ] Capture photo/video evidence from both models.
- [ ] Record separate frame-time and memory results for both models.

**Acceptance:** a buildable VPK, a visible rotating mesh and updating timing overlay, plus a reproducible physical-hardware result for each model. Do not mark M0 complete based on Vita3K alone.

## M1 — Backend comparison and renderer foundation

**New research input:** [Godot 4 PS Vita native GXM field study](GXM_FIELD_STUDY.md). This independently demonstrates a substantial native GXM 2D renderer and experimental 3D renderer, including reported physical-hardware results. It strengthens the case for direct-GXM research but does not replace our own controlled vitaGL-vs-GXM benchmark.

- [ ] Keep the same First Light scene as the comparison workload.
- [ ] Build a direct-SceGxm proof of concept.
- [ ] Compare direct GXM with vitaGL under identical conditions.
- [ ] Record CPU frame time, GPU time or a labelled proxy, memory pools and frame-time distribution.
- [ ] Choose the initial backend based on evidence and maintainability.
- [ ] Define vertex, index, mesh, camera and transform interfaces.

## M2 — Native engine foundation

- [ ] Separate platform lifecycle, input, timing and display services.
- [ ] Add a backend-neutral render interface.
- [ ] Add depth-tested mesh submission and basic texture sampling.
- [ ] Add deterministic scene loading and a repeatable test harness.
- [ ] Keep PCH-1000 and PCH-2000 in the hardware test matrix.

## M3 — Asset pipeline and diagnostics

- [ ] Choose documented runtime asset formats.
- [ ] Add host-side validation/conversion tools.
- [ ] Track main RAM and VRAM allocations separately.
- [ ] Add frame-time percentiles, draw-call and allocation diagnostics.
- [ ] Add automated build checks where the toolchain is available.

## M4 — Visual feature experiments

- [ ] Compare simple lighting models.
- [ ] Test normal mapping and material variants.
- [ ] Evaluate fog, transparency and environment rendering.
- [ ] Prototype shadows/reflections only after baseline performance is understood.
- [ ] Document visual quality and measured costs on real hardware.

## M5 — Reusable engine

- [ ] Stabilize public engine interfaces.
- [ ] Add an original sample project with no third-party game assets.
- [ ] Document how to add a renderer feature or platform service.
- [ ] Publish known limitations and benchmark methodology.

## Acceptance rule

No feature is called supported until it has a buildable implementation, a test or sample, documented limitations, and measured results where performance matters.
