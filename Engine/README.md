# Engine

This directory will contain the game-agnostic engine core.

Planned modules:

- Core types, math, and logging
- Scene and transform systems
- Camera and visibility
- Mesh and material abstractions
- Renderer orchestration
- Asset/resource handles
- Diagnostics and profiling

Keep Vita-specific APIs in Platform/Vita or Backends rather than spreading them throughout the engine.
