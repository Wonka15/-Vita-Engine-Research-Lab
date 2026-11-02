# Vita Engine Research Lab

**An independent, hardware-first 3D graphics engine research project for PlayStation Vita.**

The goal is to build original, reusable rendering technology and prove it on physical PS Vita consoles. The engine is game-agnostic; commercial games are research references only.

## Current status — M0 First Light

**Stage:** Native Vita prototype source added; build and physical-hardware verification pending.

The first prototype is [First Light](Prototypes/FirstLight/README.md): an original rotating cube rendered in a native Vita homebrew application, with an FPS and last-frame-time overlay. It uses vitaGL as a GXM-backed bootstrap path; it is not yet a direct-SceGxm renderer.

**Next proof:** build the VPK, launch it on a physical PCH-1000 (original OLED) and PCH-2000 (Slim LCD), and publish separate results. No hardware-success claim is made until each model has been tested.

## Start here

- [Getting Started](Documentation/GETTING_STARTED.md) — VitaSDK setup on Windows/WSL2, build and install steps.
- [First Light prototype](Prototypes/FirstLight/README.md) — source, dependency notes and build instructions.
- [Contributing](CONTRIBUTING.md) — how to help and what a hardware report must include.
- [Roadmap](Documentation/ROADMAP.md) — milestone acceptance criteria.
- [Decision log](Documentation/DECISION_LOG.md) — why vitaGL is used first and how direct GXM will be compared.
- [Benchmark scene](Tests/SCENES/FIRST_LIGHT.md) and [results template](Tests/RESULTS_TEMPLATE.md).

## Hardware targets

| Target | Display | Test requirement |
|---|---|---|
| PCH-1000 (original/Fat) | 5-inch OLED | Separate physical-device run and evidence |
| PCH-2000 (Slim) | 5-inch LCD | Separate physical-device run and evidence |

Both use the Vita's quad-core Cortex-A9 / SGX543MP4+ platform and 512 MB main RAM plus 128 MB VRAM. Shared core architecture does not remove the need to test each revision independently.

## Engineering principles

- Physical hardware is the primary performance authority.
- Vita3K is useful for launch and regression checks, not hardware performance claims.
- Keep platform integration, graphics backend, renderer, assets and tools modular.
- Measure CPU and GPU bottlenecks before optimizing.
- Keep fixed, reproducible benchmark scenes.
- Label results as build-tested, emulator-tested, or hardware-tested for a named model.
- Never commit proprietary game assets, source, extracted data, firmware modules or console keys.

## Backend direction

The first mesh uses [vitaGL](https://github.com/Rinnegatamante/vitaGL), an OpenGL subset translated to SceGxm. A direct GXM backend remains a planned comparison. See the [decision log](Documentation/DECISION_LOG.md) for the fair-test criteria and third-party dependency caveats.

## Research library

The [technical reference library](https://wonka15.github.io/-Vita-Engine-Research-Lab/references.html) collects Vita graphics APIs, commercial game studies, open-source projects, engine lineage, performance architecture and licensing notes. The [browser graphics lab](https://wonka15.github.io/-Vita-Engine-Research-Lab/browser-lab.html) is a separate desktop/browser sandbox—not a Vita emulator or hardware benchmark.

## License and independence

Original project code is licensed under the repository's MIT license. Third-party dependencies retain their own licenses and notices; linking or distributing a combined application may impose additional obligations. This is an independent project and is not affiliated with Sony, Microsoft, Bungie or 343 Industries.
