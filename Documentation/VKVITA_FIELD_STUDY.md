# VkVita Field Study — Vulkan-Style API on SceGxm

**Upstream:** [DrDecki/vkvita](https://github.com/DrDecki/vkvita)  
**Inspected:** 2 November 2026  
**Upstream license:** GPL-3.0-or-later  
**Research status:** Source and README reviewed; not integrated, built, or benchmarked in this engine.

## What it is

VkVita is a static library that implements a large subset of the Vulkan 1.0 `vk*` API for PlayStation Vita homebrew and maps those calls directly to the Vita's SceGxm graphics system. It is not a native Vulkan driver, Vulkan loader, or hardware implementation of the complete Vulkan specification. The upstream README describes SPIR-V shader translation to Cg, with shaders compiled on-console through vitaShaRK or supplied as precompiled GXP binaries.

This is especially interesting to our engine because it explores a different abstraction point from both vitaGL and our planned direct-SceGxm renderer:

- **vitaGL:** OpenGL-style application interface translated to GXM.
- **VkVita:** Vulkan-style application interface mapped to GXM.
- **Direct SceGxm:** Our renderer submits work to the native Vita graphics API directly.

## Why it matters to our architecture

VkVita could let an engine share a Vulkan-oriented renderer or graphics layer with other platforms while providing a Vita backend. That is an attractive architectural direction for a cross-platform engine. It may also offer more explicit command-buffer, resource and pipeline concepts than an OpenGL-style API.

However, API familiarity does not guarantee performance. VkVita still has to translate Vulkan semantics into the capabilities and constraints of SceGxm. The wrapper may add CPU work or impose compatibility limits; it may also provide useful batching, resource management and portability. We need measurements rather than assumptions.

## Upstream capabilities described in its README

- Static library `libvkvita.a`; CMake package and pkg-config integration.
- Vulkan 1.0-style API subset mapped to SceGxm.
- SPIR-V shader path through SPIRV-Cross and Cg translation.
- Runtime shader compilation through vitaShaRK when enabled, or precompiled GXP shader packs.
- Debug logging, validation, shader dumps, profiling and optional GPU synchronization diagnostics.
- Configurable vsync behaviour, shader cache, memory pools and display queue depth.
- Samples include a triangle, textured cube, fixed-function layer, SPIR-V cube, GPU feature tests, subpasses, MSAA and a standalone template.

These are upstream README claims and feature descriptions, not independently verified results from our own builds.

## Important limitations and prerequisites

The upstream documentation explicitly lists platform/API differences:

- The display/swapchain is fixed at 960 × 544.
- Compute shaders / compute functionality are not available on GXM.
- One colour attachment per subpass is supported.
- Multisampling is listed for 2× and 4×.
- Storage buffers are supported only for read-only shader use.
- Mipmaps require power-of-two textures.
- Not every SPIR-V feature has a GXM counterpart.

Runtime SPIR-V/Cg compilation requires vitaShaRK and a decrypted `libshacccg.suprx` on the console. The upstream README describes a precompiled GXP shader route when built with `VKV_WITH_SHARK=OFF`, which avoids that runtime requirement.

The repository currently states that it has no formal releases and that the `main` branch is the current build. Pin a commit before any reproducible experiment.

## Licensing and dependency boundary

VkVita is GPL-3.0-or-later. Its README also identifies third-party components with separate terms, including Vulkan-Headers (Apache-2.0 or MIT), SPIRV-Cross (Apache-2.0) and vitaShaRK (LGPL-3.0). Review the exact checked-out revisions, submodule licenses, notices and distribution implications before linking or bundling it.

Our engine's original code remains MIT-licensed. This study does not copy VkVita source, headers, shaders or binaries into our repository, and does not make VkVita a project dependency.

## Proposed experiments

1. Pin an upstream commit and build VkVita using the documented VitaSDK workflow.
2. Build and run the upstream `textured_cube` and `spirv_cube` samples, first in Vita3K for smoke testing and then on a physical Vita.
3. Port our original First Light cube to VkVita without changing its geometry, camera, colour/depth formats or animation.
4. Compare three backends where practical: vitaGL, VkVita and direct SceGxm.
5. Keep the same physical console, firmware, resolution, frame-pacing policy, warm-up and capture duration.
6. Record CPU frame time, FPS distribution, memory use, draw/submit counts, shader compilation/startup cost, visual parity and unsupported features.
7. Separate shader compilation time from steady-state rendering; test both cached/precompiled and runtime-compiled shader paths where possible.

Do not interpret a successful build or a sample's FPS as evidence that VkVita is faster than vitaGL or direct GXM.

## Current decision

**Add VkVita to the research library as a candidate abstraction/backend study. Do not adopt it as the engine's default renderer yet.** Its cross-platform Vulkan-style API is promising, but the compatibility subset, GPL-3.0-or-later license, runtime shader prerequisites and real-hardware performance need to be evaluated first.

## References

- [VkVita upstream repository and README](https://github.com/DrDecki/vkvita)
- [VitaSDK](https://vitasdk.org/)
- [VitaSDK API documentation](https://docs.vitasdk.org/)
