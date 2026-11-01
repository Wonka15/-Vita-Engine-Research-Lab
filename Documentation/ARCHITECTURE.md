# Engine architecture

## Design target

Halo3DVita is a reusable 3D engine for PlayStation Vita. The design separates platform services, graphics backends, renderer systems, and content tools so that experiments do not lock the project into one game or one rendering API.

## Proposed layers

1. **Platform/Vita** — application lifecycle, input, timers, display integration, file access, and Vita-specific services.
2. **Graphics backend** — GPU resources, command submission, render targets, synchronization, and shader/program handling.
3. **Renderer** — camera, transforms, meshes, materials, render queues, visibility, and frame orchestration.
4. **Engine systems** — scenes, asset handles, animation, resource management, and diagnostics.
5. **Tools** — host-side asset preparation, validation, and benchmark reporting.

## Backend strategy

Investigate two routes with small proof-of-concept programs:

- **Native GXM:** direct access to Vita's GPU interface through VitaSDK. This is the likely route for low-level control and platform-specific optimization.
- **vitaGL:** an OpenGL-style layer over GXM that may accelerate prototyping or portability, with its own limitations and overhead.

Do not choose based on assumptions. Compare startup complexity, supported features, draw-call cost, shader workflow, resource management, and measured performance on device.

## Rendering feature tiers

Every expensive feature should have a quality tier or fallback.

- Baseline: textured meshes, depth testing, opaque geometry, simple directional lighting.
- Enhanced: normal mapping, multiple light sources, fog, and selected transparency effects.
- Experimental: shadow techniques, reflections, post-processing, and other costly effects.

These are research categories, not promises that every effect will be practical at full resolution or in every scene.

## Memory and performance

Treat CPU memory, GPU-visible allocations, texture storage, render targets, and command buffers as explicit budgets. Record resolution, scene complexity, texture formats, draw calls, and frame timing with every benchmark.

## Content boundary

Game-specific formats, if ever investigated, belong in optional import adapters or research documentation. The core engine must not depend on proprietary Halo code or bundled game assets.
