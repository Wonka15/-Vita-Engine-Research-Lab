# Halo3DVita

**An independent, reusable 3D graphics engine project for PlayStation Vita.**

Halo3DVita is a hardware-first research and development project exploring how far the PlayStation Vita can be pushed as a 3D graphics platform. Halo is one of the project's visual research references, but the engine is intended to support original scenes, tools, experiments, and potentially many kinds of games.

## Vision

The project pursues three goals:

1. **Push Vita hardware:** make careful use of the Vita CPU, GPU, memory, and display through profiling and targeted optimization.
2. **Explore modern rendering techniques:** investigate advanced materials, lighting, shadows, reflections, post-processing, and environmental effects, adapting each technique to what the hardware can realistically handle.
3. **Build a reusable engine:** create modular rendering, asset, scene, and tooling systems that can serve projects beyond Halo.

## Project principles

- Hardware-first design: measure performance on real hardware wherever possible.
- Modular architecture: keep rendering, assets, platform code, and tools separate.
- Scalable effects: provide simpler fallbacks for expensive visual features.
- Reproducible tests: benchmark changes with documented scenes and settings.
- Honest documentation: distinguish verified behavior from experiments and assumptions.
- Respect copyrights: do not commit proprietary game assets, source code, executable files, or extracted content.

## Planned engine systems

| System | Responsibility |
|---|---|
| Platform layer | Vita startup, input, timing, display, and system integration |
| Graphics backend | GPU initialization, command submission, buffers, textures, and synchronization |
| Renderer | Cameras, meshes, materials, render queues, and scene drawing |
| Lighting and effects | Lighting models, shadows, reflections, fog, and post-processing experiments |
| Asset pipeline | Import/conversion tools and runtime-friendly asset formats |
| Scene system | Scene graph, transforms, visibility, and test environments |
| Animation | Skeletal animation and interpolation experiments |
| Memory and profiling | Allocation strategy, GPU/CPU timings, memory budgets, and performance reports |

## Graphics research references

Halo: Combat Evolved, Halo 2, and Halo 3 may be used as visual and technical research references for studying different rendering approaches. They are not separate game-port targets in this repository. The engine should remain game-agnostic.

## Initial technical direction

- **Primary toolchain:** [VitaSDK](https://vitasdk.org/)
- **Low-level graphics research:** Sony's Vita GXM interface as exposed by VitaSDK.
- **Optional graphics abstraction:** evaluate [vitaGL](https://github.com/Rinnegatamante/vitaGL) for portability and rapid prototyping.
- **Build system:** CMake, following current VitaSDK samples.
- **First milestone:** a minimal Vita application that clears the screen, draws a rotating 3D mesh, accepts basic camera input, and reports frame timing.

The graphics backend decision will be validated with small benchmarks before the engine commits to a permanent abstraction. Modern desktop rendering features must not be assumed to map directly to Vita hardware.

## Repository layout

```text
Engine/          Core engine modules and renderer
Platform/Vita/   Vita-specific platform integration
Backends/        Graphics backend experiments
Assets/          Original test assets and asset-pipeline samples
Tools/           Host-side conversion and development tools
Tests/           Repeatable rendering and performance tests
Research/        Hardware notes and graphics experiments
Documentation/   Architecture, setup, and development notes
```

## Development status

**Stage:** Planning and technical foundation.

No production renderer or performance claims are made yet. The first goal is a small, measurable graphics prototype, followed by a modular engine architecture.

See:
- [Architecture](Documentation/ARCHITECTURE.md)
- [Roadmap](Documentation/ROADMAP.md)
- [Development setup](Documentation/DEVELOPMENT.md)
- [Performance testing](Tests/README.md)

## Legal and project status

This is an independent community project and is not affiliated with, endorsed by, or sponsored by Microsoft, 343 Industries, Bungie, or Sony Interactive Entertainment. Halo and related names are the property of their respective owners. This repository should contain original code and documentation only; users must provide their own legally obtained game content where applicable.

The repository's MIT license applies to original project code only. It does not grant rights to third-party trademarks, game assets, or copyrighted material.
