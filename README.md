# Vita Engine Research Lab

**An independent, hardware-first 3D graphics engine research project for PlayStation Vita.**

The goal is to build original, reusable rendering technology and prove it on physical PS Vita consoles. The engine is game-agnostic; commercial games are research references only.

<p align="center"><a href="https://wonka15.github.io/-Vita-Engine-Research-Lab/browser-lab.html"><strong>▶ OPEN THE BROWSER GRAPHICS LAB</strong></a></p>

Explore the rotating First Light 3D cube, adjust its camera and rotation, and watch live browser FPS/frame-time measurements. This is a visual reference—not a Vita emulator or a substitute for physical-hardware benchmarks.

## Download First Light Alpha

**Want to try it on a PS Vita?** The latest confirmed successful Alpha package is available from GitHub Actions:

**[Download First Light Alpha 0.1.0 (VPK inside ZIP)](https://github.com/Wonka15/-Vita-Engine-Research-Lab/actions/runs/41366347176)**

1. Open the successful build run above.
2. Scroll to **Artifacts** near the bottom of the page.
3. Download **Vita-Engine-First-Light-Alpha-0.1.0**.
4. Extract the ZIP to find the `.vpk` installer.
5. Install it using VitaShell on a homebrew-enabled PS Vita.

The Actions artifact is temporary and currently expires **2 December 2026**. This is a build-tested package, not yet verified on a physical Vita. The newer benchmark HUD polish is being rebuilt separately; check the [latest build status](https://github.com/Wonka15/-Vita-Engine-Research-Lab/actions/workflows/vita-build.yml) for updates.

## Current status — M0 First Light

**Stage:** First Light Alpha VPK successfully built by GitHub Actions; physical-hardware verification pending.

The first prototype is [First Light](Prototypes/FirstLight/README.md): an original rotating cube rendered in a native Vita homebrew application, with an FPS and last-frame-time overlay. It uses vitaGL as a GXM-backed bootstrap path; it is not yet a direct-SceGxm renderer.

**Next proof:** install and launch the VPK on a physical PCH-1000 (original OLED) and PCH-2000 (Slim LCD), then publish separate results. No hardware-success claim is made until each model has been tested.

## Start here

- [Browser Graphics Lab](https://wonka15.github.io/-Vita-Engine-Research-Lab/browser-lab.html) — interactive browser visualization of the First Light cube.
- [NV2A Shader Translation Lab](https://wonka15.github.io/-Vita-Engine-Research-Lab/shader-lab.html) — interactive study of Xbox vertex instructions, pixel combiners, texture stages and the D3D8-to-GXM pipeline.
- [Getting Started](Documentation/GETTING_STARTED.md) — VitaSDK setup on Windows/WSL2, build and install steps.
- [First Light prototype](Prototypes/FirstLight/README.md) — source, dependency notes and build instructions.
- [Contributing](CONTRIBUTING.md) — how to help and what a hardware report must include.
- [Roadmap](Documentation/ROADMAP.md) — milestone acceptance criteria.
- [Decision log](Documentation/DECISION_LOG.md) — why vitaGL is used first and how direct GXM will be compared.
- [Godot 4 Vita GXM field study](Documentation/GXM_FIELD_STUDY.md) — research evidence from WolffsRoom's native GXM engine port.
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

The first mesh uses [vitaGL](https://github.com/Rinnegatamante/vitaGL), an OpenGL subset translated to the Vita's SceGxm GPU. A direct GXM backend is now an active research path. The [Godot 4 PS Vita port by WolffsRoom](https://github.com/WolffsRoom/godot4-psvita) demonstrates a substantial native GXM 2D renderer and experimental 3D renderer, with reported physical-hardware results. This supports studying direct GXM seriously, but does not prove a universal speed advantage over vitaGL; our own controlled comparison remains necessary. See the [decision log](Documentation/DECISION_LOG.md) for the fair-test criteria and third-party dependency caveats.

## Research library

The [technical reference library](https://wonka15.github.io/-Vita-Engine-Research-Lab/references.html) collects Vita graphics APIs, commercial game studies, open-source projects, engine lineage, performance architecture and licensing notes. The [browser graphics lab](https://wonka15.github.io/-Vita-Engine-Research-Lab/browser-lab.html) is a separate desktop/browser sandbox—not a Vita emulator or hardware benchmark. The [NV2A Shader Translation Lab](https://wonka15.github.io/-Vita-Engine-Research-Lab/shader-lab.html) provides a focused interactive investigation area for the Xbox-to-Vita shader translation concepts.

## License and independence

Original project code is licensed under the repository's MIT license. Third-party dependencies retain their own licenses and notices; linking or distributing a combined application may impose additional obligations. This is an independent project and is not affiliated with Sony, Microsoft, Bungie or 343 Industries.
