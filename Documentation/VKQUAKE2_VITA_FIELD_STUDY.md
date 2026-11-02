# vkQuake2 Vita Field Study — A Real Vulkan Renderer Through VkVita

**Upstream port:** [DrDecki/vkquake2-vita](https://github.com/DrDecki/vkquake2-vita)  
**Upstream renderer:** [kondrak/vkQuake2](https://github.com/kondrak/vkQuake2)  
**Graphics compatibility layer:** [DrDecki/vkvita](https://github.com/DrDecki/vkvita)  
**Inspected:** 2 November 2026  
**Port license:** GPL-2.0-or-later (as stated by the port README and repository metadata)  
**Research status:** README and repository metadata reviewed; not built, benchmarked or tested by Vita Engine Research Lab.

## Why this project matters

This is a particularly valuable companion study to VkVita itself. The port brings the Vulkan renderer from vkQuake2 to PlayStation Vita through VkVita, which maps Vulkan-style calls onto the Vita's native SceGxm graphics system.

The port README explicitly states that it uses the actual vkQuake2 Vulkan renderer and SPIR-V shaders, rather than switching to a software renderer or an OpenGL renderer. Because the Vita GPU does not execute SPIR-V natively, VkVita translates the shaders to Cg and compiles them. The port ships compiled shaders in its VPK, so the README says the shader compiler does not need to be installed on the console for normal use.

This gives us a concrete application-level example of the VkVita architecture, not just an API wrapper or isolated graphics sample.

## What the upstream README reports

- The full Quake II single-player campaign.
- Wi-Fi deathmatch between Vita systems and between Vita and PC clients.
- Sound effects and OGG soundtrack.
- 2× MSAA by default, with 4× available.
- A developer-reported target/result of 60 FPS in almost all areas; the README notes that 4× MSAA has less headroom in large fights.

These are claims in the upstream README, not measurements independently reproduced by our project. Do not treat them as a controlled comparison with vitaGL, direct GXM or our First Light prototype.

A third-party homebrew listing also describes the release as running at 960×544 with 2× MSAA and reports 60 FPS in most areas. This is corroborating publication context, not independent benchmark evidence from our lab.

## Architecture and integration points

The port is based on the Vulkan renderer in [kondrak/vkQuake2](https://github.com/kondrak/vkQuake2), with Vita-specific code in the `vita` directory and platform changes elsewhere marked with `__vita__`, according to its README.

The documented build flow uses VitaSDK and a checkout of VkVita:

```sh
cmake -S vita -B build-vita -DVKVITA_DIR=/path/to/vkvita
cmake --build build-vita -j
```

The expected output is `build-vita/vkquake2.vpk` with title ID `VKQUAKE02`.

The repository also documents a Linux host build against a VkVita test mock. That is a useful development pattern: catch application crashes and test parts of the integration on the host before deploying to Vita. It does not replace a real-device test of GXM behaviour.

## Shader pipeline lesson

The port's shader workflow is especially relevant:

1. Start with the upstream Vulkan/SPIR-V shader path.
2. Translate SPIR-V to the Vita-compatible Cg path through VkVita.
3. Compile shaders for the Vita GPU.
4. Package the compiled shader files into the VPK.
5. Rebuild and regenerate the packaged shaders if a VkVita change alters their output.

The README describes a runtime regeneration route using `libshacccg.suprx`, followed by copying generated shader files from the application's data directory into the source shader pack. For the normal packaged build, the precompiled shaders are shipped with the VPK.

This suggests a useful engine architecture distinction: shader authoring/translation/compilation can be a host-side or controlled preparation pipeline, while the shipped Vita runtime loads device-ready shader binaries.

## Input, networking and platform integration

The README documents Vita-specific dual-stick controls, remappable buttons, Wi-Fi server hosting and joining, direct IP connection, and interoperability with PC Quake II clients using the original network protocol.

These features make the repository useful beyond graphics. It can inform our research into:
- Vita input mapping and right-stick camera control.
- Network lifecycle and local-network discovery.
- Logging and crash diagnosis.
- Packaging runtime assets and configuration.
- Separating platform-specific integration from the shared game/renderer code.

The README says logs are written under `ux0:data/vkquake2/`, including separate game and VkVita renderer logs.

## Important boundaries and caveats

- The Vita has no native Vulkan driver. This project does not prove that Vulkan runs directly on Vita hardware; it demonstrates a Vulkan API implementation/translation layer over SceGxm.
- The reported 60 FPS is an upstream claim, not a lab-verified benchmark. The README itself notes reduced headroom at 4× MSAA in large fights.
- Quake II game data is not included. The user must supply their own legally obtained game files. Our research repository must not include retail `.pak` files or other copyrighted game assets.
- The port is GPL-2.0-or-later according to its README and repository metadata. The upstream vkQuake2 project is also GPL-2.0. Review the exact source revision, notices, submodules and dependencies before incorporating or redistributing code.
- The port is a fork of the upstream vkQuake2 codebase. Keep attribution and license provenance explicit.
- A successful VPK launch or game playthrough would establish compatibility, not prove that VkVita is faster or more efficient than another backend.

## What this teaches our engine project

This project strengthens the case for investigating a backend-neutral renderer interface. It shows that an established Vulkan renderer can be adapted to Vita by implementing a platform translation layer and a shader conversion/package workflow, while retaining much of the upstream renderer.

For our engine, the key architectural questions are:
- Can our renderer target a narrow Vulkan-like API subset that maps cleanly to VkVita?
- Which Vulkan concepts are genuinely useful on Vita, and which become expensive or awkward when translated to GXM?
- Can a shared renderer core serve desktop and Vita without forcing desktop constraints onto the Vita backend?
- How much CPU submission overhead, memory use and shader complexity does the abstraction add?
- Would a smaller engine-specific render interface be more portable and maintainable than adopting a broad Vulkan API surface?

Do not copy Quake II code or assets into our engine. Study the integration boundary and implement original tests.

## Proposed experiments

1. Pin exact commits of vkQuake2 Vita and VkVita; record the toolchain and submodule revisions.
2. Reproduce the documented Vita VPK build.
3. Run the upstream build's host/mock test path and record what it does and does not validate.
4. Launch the VPK in Vita3K for smoke testing, then on physical Vita hardware.
5. Record resolution, MSAA, frame pacing, scene/location, device model and firmware.
6. Capture sustained frame-time distributions in repeatable scenes, including a demanding combat scene; separate average FPS from frame pacing and 1% lows.
7. Compare 2× and 4× MSAA to understand the reported headroom trade-off.
8. Build an original First Light scene through VkVita; do not use Quake II game assets.
9. Compare the same scene on vitaGL, VkVita and direct GXM, using the same physical device and test protocol.
10. Record CPU frame time, GPU time where measurable (or a clearly labelled proxy), memory, draw/submit counts, shader startup/cache behaviour and visual parity.

## Current decision

**Add vkQuake2 Vita as an application-level reference for VkVita. Do not adopt the port or its game code as an engine dependency.**

It is currently one of the most useful examples in this research library because it demonstrates a complete, recognizable 3D game renderer and shader pipeline using VkVita on Vita. Its reported performance is interesting, but we need our own reproducible build and controlled physical-hardware measurements before drawing performance conclusions.

## References

- [vkQuake2 Vita port](https://github.com/DrDecki/vkquake2-vita)
- [VkVita compatibility layer](https://github.com/DrDecki/vkvita)
- [Upstream vkQuake2 renderer](https://github.com/kondrak/vkQuake2)
- [VitaSDK](https://vitasdk.org/)
