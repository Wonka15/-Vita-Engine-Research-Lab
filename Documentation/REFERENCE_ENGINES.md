# Reference Engines: Killzone: Mercenary and OpenStrike

**Status:** Research and implementation guide  
**Project:** Vita Engine Research Lab  
**Purpose:** Learn from two complementary PS Vita references while building an original, reusable engine.

> **Engineering principle:** Study the best. Respect the creators. Build our own technology. Prove everything on the hardware.

This document defines two major reference tracks for Vita Engine Research Lab:

1. **Killzone: Mercenary** is the visual and performance reference: an example of an ambitious, high-fidelity PS Vita game.
2. **OpenStrike** is the inspectable engineering reference: an open-source project with a native PS Vita target and a multi-platform architecture.

Neither project is a codebase to copy wholesale. Vita Engine Research Lab remains an independent engine with its own architecture, original code, original test assets, and separately documented design decisions.

---

## 1. Reference roles

| Reference | What we study | What we build ourselves |
|---|---|---|
| Killzone: Mercenary | Visual quality, rendering features, asset optimisation, level visibility, performance trade-offs | Original rendering experiments and benchmark scenes |
| OpenStrike | Vita platform integration, GXM host, build and packaging workflow, resource lifecycle, simulation/render separation, automated tests | Independent platform layer, renderer, asset pipeline and test harness |
| Vita Engine Research Lab | Our own design and measured results | A reusable engine for original projects |

The two references should inform different questions. Killzone helps us ask **what visual results are worth pursuing?** OpenStrike helps us ask **how can a native Vita application be structured, built, tested and maintained?**

## 2. Killzone: Mercenary — visual and performance reference

### 2.1 What public developer material tells us

In a February 2013 PlayStation Blog interview, Guerrilla Cambridge's Piers Jackson described Mercenary as using the Killzone engine architecture, with its core AI based on Killzone 3. He said some audio and rendering systems had to be replaced or adapted for Vita, and specifically mentioned floor reflection mapping and lit particle systems.

A March 2013 PlayStation Blog article described the game as running a modified Killzone 3 rendering engine and highlighted volumetric lighting and smoke, high-resolution environment textures, reflective metal and realistic-looking shadows.

A 2013 PocketGamer production interview adds useful optimisation context: the team worked to maintain an acceptable frame rate through engine, asset and mission-design optimisation. The interview discusses aggressive UV layout and LOD work to reduce texture memory and distant triangle counts, as well as character visibility and rim lighting. These are reported developer statements, not a complete technical specification.

### 2.2 Research topics for Halo3DVita

| Topic | Research question | Independent experiment |
|---|---|---|
| Floor reflections | What quality/cost trade-off can a reflection technique achieve on Vita? | Compare static environment reflection, planar reflection and a cheaper screen-space approximation where feasible |
| Lit particles | How can particles respond to scene lighting without excessive CPU/GPU cost? | Small particle test with unlit, vertex-lit and simplified per-particle lighting variants |
| Volumetric appearance | Can fog, light shafts or smoke suggest depth at a lower cost? | Layered fog cards, low-resolution light shafts, and simple depth-based fog |
| Materials | How can metal, roughness and surface response be represented efficiently? | Small material gallery with a limited shader feature set |
| Shadows | What shadow approaches fit Vita memory and fill-rate limits? | Compare baked shadows, blob/contact shadows and a small shadow-map prototype |
| Texture memory | How can UVs, atlases, mip levels and texture formats reduce memory use? | Asset variants with recorded dimensions, format, memory footprint and visual result |
| LOD and visibility | How can distant geometry and characters be simplified without distracting transitions? | LOD test scene with screen-space thresholds and visibility counters |
| Scene design | How does level layout affect visibility, overdraw and workload? | Repeated test layouts with different occlusion, depth complexity and object counts |
| Frame-rate stability | Which workloads cause frame-time spikes? | Repeatable stress scene with frame-time distribution and workload counters |

### 2.3 Important interpretation

Mercenary demonstrates that the Vita can deliver a striking visual result when engine systems, content and level design are optimised together. It does **not** mean every effect shown in a PS3 engine is automatically affordable on Vita, or that the original implementation details are publicly known.

We should not assume Mercenary used a particular implementation unless a credible public source confirms it. For example, a public mention of reflection mapping does not establish that it was screen-space reflection. Our experiments should label implementation choices as our own hypotheses.

### 2.4 Proposed visual benchmark scene

Create an original industrial test environment containing:

- A few large architectural forms and modular props.
- One reflective floor section and several material types.
- A directional light and a small number of local lights.
- Fog or smoke layers and a controlled particle emitter.
- A moving character-sized test object for visibility and LOD evaluation.
- A static camera route that can be replayed consistently.

Use original geometry, textures and sounds. Do not extract or reproduce Killzone assets, levels, shaders, source code or proprietary tools.

Record for every run:

- Device or emulator and software version.
- Resolution and render-target formats.
- Average, median, 95th-percentile and worst frame time.
- CPU/GPU timing where measurable.
- Draw calls, submitted triangles and visible objects.
- Texture and render-target memory estimates.
- Enabled effects and quality settings.
- Visual capture and known limitations.

A visual feature is not considered successful merely because it appears in a screenshot. It must also have a measured cost and a documented fallback.

---

## 3. OpenStrike — engineering and implementation reference

Repository: https://github.com/pocket-nexus/open-strike

OpenStrike's repository describes a multi-platform FPS project with a native PS Vita VPK target. Its README documents a shared Rust simulation, a Pocket3D rendering layer, a PocketJS/QuickJS guest for game rules and HUD, and platform-specific hosts. Its Vita README describes a 960×544 native framebuffer, a 480×272 logical UI viewport rendered at 2× density, a 60 Hz simulation with interpolation, Vita3K end-to-end tests and optional wired development tooling.

These are descriptions in the upstream repository. Vita Engine Research Lab should verify current implementation details against the upstream source and current revisions before relying on them.

### 3.1 Areas worth studying

| OpenStrike area | Why it matters | Research Lab investigation |
|---|---|---|
| Vita host and GXM integration | Real native graphics API integration | Trace startup, display setup, frame loop, command submission and shutdown |
| Platform-specific crate | Separates Vita-specific code from shared systems | Define a narrow platform interface rather than spreading Vita calls through engine code |
| Shared simulation | Keeps game rules independent from rendering target | Keep engine simulation and renderer APIs decoupled where useful |
| Pocket3D world/render layer | Demonstrates a dedicated 3D runtime for Vita | Study data flow and resource ownership; do not assume the library is reusable under our license |
| Frame interpolation | Smooths rendered motion between fixed simulation updates | Prototype fixed-step update plus render interpolation |
| Resource lifecycle | GPU resources must be retired safely when scenes change | Document ownership, allocation, release and device-loss/shutdown paths |
| Build and packaging scripts | Repeatable builds reduce toolchain drift | Build a reproducible VitaSDK pipeline with explicit tool versions |
| Vita3K E2E and golden captures | Regression testing catches visual or runtime breakage | Add deterministic scene captures and structural counters |
| USB development workflow | Faster iteration on hardware | Consider later, only after a safe and well-defined development protocol exists |

### 3.2 Suggested source-reading order

Study the upstream repository in small, focused passes:

1. Root README and license.
2. `crates/openstrike-vita/README.md` for the Vita-specific build and runtime overview.
3. `scripts/vita.ts` for build orchestration and packaging.
4. `scripts/e2e-vita.ts` and Vita golden tests for emulator testing.
5. Vita crate source for application startup, input, frame loop and rendering calls.
6. Shared simulation crate to understand how game logic is separated from platform rendering.
7. The vendored PocketJS submodule and Pocket3D-related code only after reviewing their own license terms.

Do not copy files or code during this research pass. First produce notes describing the architecture, important functions, dependencies, and questions that need verification. If code reuse is later proposed, perform a fresh license and provenance review for the exact file and revision.

### 3.3 What not to assume

- OpenStrike's existence does not establish that its dependencies are all MIT-licensed.
- The root OpenStrike LICENSE does not automatically relicense submodules, vendored projects, assets, maps, fonts, libraries or generated content.
- A successful Vita3K test is not proof of performance or correctness on physical hardware.
- An implementation detail that works for OpenStrike's runtime may not fit the Research Lab's architecture or goals.
- A rendering library's license and redistribution conditions must be checked independently before linking or distributing it.

---

## 4. Licensing, attribution and intellectual-property boundaries

Respecting upstream creators is a project requirement, not an afterthought.

### 4.1 OpenStrike

The OpenStrike repository's root LICENSE file identifies the project as MIT licensed and names “pocket-stack contributors” in its copyright notice. The MIT license permits reuse subject to its conditions, including retaining the copyright and permission notice in copies or substantial portions of the software.

However, OpenStrike's README identifies a vendored PocketJS submodule, and its Vita documentation refers to Pocket3D licensing requirements. Submodules and third-party dependencies retain their own licensing terms. The root MIT license must not be treated as blanket permission to copy or redistribute every component in the repository.

**Research Lab policy:**

- Prefer learning from architecture and independently implementing the idea.
- Do not copy code merely because the top-level repository says MIT.
- Before any code reuse, record the exact upstream repository, file path, commit/revision, author/copyright holder, license and required notices.
- Preserve all required copyright, license and attribution notices.
- Review submodule, dependency, asset, font, map and tool licenses separately.
- Do not redistribute OpenStrike maps, game data or third-party assets.
- If a component's license is missing, ambiguous or incompatible, do not incorporate it until clarified.

### 4.2 Killzone: Mercenary and other proprietary games

Killzone, its engine, game assets, characters, levels, audio, trademarks and proprietary development tools belong to their respective rights holders.

**Research Lab policy:**

- Use public interviews, articles and presentations as research references.
- Do not copy proprietary source code, shaders, tools, extracted assets, textures, maps, models, audio or game data.
- Do not present Halo3DVita as affiliated with Guerrilla, Sony, Microsoft, Bungie or 343 Industries.
- Use original names, original assets and original implementation.
- Describe visual goals as “inspired by” or “benchmarked against” only where appropriate; do not imply endorsement.

### 4.3 Provenance record for any future reuse

Before accepting third-party code or assets, add a record containing:

| Field | Required information |
|---|---|
| Component | Exact file, library or asset |
| Origin | Canonical repository or source URL |
| Revision | Commit SHA, release or version |
| Creator | Author or copyright holder |
| License | Exact license identifier and link |
| Changes | Original, modified or generated |
| Obligations | Notices, attribution, source disclosure, distribution conditions |
| Decision | Approved, rejected or pending review |

This is an engineering checklist, not legal advice. If a proposed reuse has unclear or consequential licensing implications, obtain qualified legal advice.

---

## 5. Vita Engine Research Lab implementation strategy

### Track A — platform foundation

- Establish a minimal native Vita application.
- Record startup, display, input, timing and shutdown behavior.
- Build a small GXM test before choosing a larger renderer architecture.
- Keep platform-specific code behind a small interface.
- Track all toolchain versions and build inputs.

### Track B — renderer foundation

- Draw a triangle, then a textured mesh.
- Add camera and transform handling.
- Add depth testing, culling and basic material state.
- Implement a clear resource ownership and lifetime model.
- Record CPU time, GPU time where available, draw calls, triangles and memory.

### Track C — Mercenary-inspired visual research

Implement one experiment at a time, in this order:

1. Texture formats, UV layouts and LOD.
2. Material response and a simple lighting model.
3. Fog and layered smoke.
4. Lit particles.
5. Reflection approximation.
6. Shadow approaches.
7. More advanced combinations only when measurements justify them.

Each experiment should have a low-cost fallback and a benchmark result.

### Track D — OpenStrike-informed engineering research

- Map the upstream Vita application lifecycle.
- Understand its rendering submission and resource retirement path.
- Examine deterministic emulator tests and golden captures.
- Compare its build process with a clean VitaSDK sample.
- Write independent design notes before implementing similar systems.

### Track E — validation

Use three levels of validation:

1. **Browser graphics lab:** fast visual prototyping and interaction. This is not a Vita emulator.
2. **Vita3K:** native VPK launch, runtime smoke tests and reproducible captures. Emulator results are not physical-hardware performance results.
3. **Physical PS Vita:** final validation for frame timing, memory pressure, GPU behavior, input and compatibility.

Never label an optimisation “Vita-verified” until it has been measured on physical Vita hardware.

---

## 6. Proposed benchmark suite

| Benchmark | Controlled variables | Measurements |
|---|---|---|
| Geometry load | Mesh count, triangle count, material count | Frame time, draw calls, submitted triangles |
| Texture pressure | Texture dimensions, formats, mip levels | Memory footprint, load time, visual quality |
| Overdraw | Layer count, transparent surfaces, particle coverage | Frame time, fill-rate symptoms, visual artifacts |
| Lighting | Light count, shader complexity | Frame time and image comparison |
| Particles | Particle count, size, blending mode | CPU update cost, draw calls, frame time |
| Reflections | Technique, resolution, update frequency | Render-target memory and frame time |
| Shadows | Technique, resolution, caster count | Frame time, memory and artifacts |
| LOD/visibility | Distance thresholds, object count | Submitted triangles and transition quality |
| Scene transitions | Resource sizes and repeated loads | Peak memory, load time, leaks or stale resources |

Keep test scenes, settings and capture paths version-controlled. A benchmark report should include the exact build revision and test conditions.

---

## 7. Immediate milestones

### Milestone 1 — reference study
- [ ] Read the OpenStrike root README and license.
- [ ] Read its Vita README and map the documented build/test flow.
- [ ] Identify PocketJS/Pocket3D submodule and dependency license boundaries.
- [ ] Extract publicly documented Killzone: Mercenary rendering and optimisation goals.
- [ ] Record unknowns rather than guessing proprietary implementation details.

### Milestone 2 — original test scene
- [ ] Create a small original industrial scene.
- [ ] Add a textured mesh and camera.
- [ ] Add a fog or smoke experiment.
- [ ] Add a particle experiment.
- [ ] Add a reflection experiment.
- [ ] Add a simple LOD test.
- [ ] Record baseline performance.

### Milestone 3 — native validation
- [ ] Build the minimal Vita application.
- [ ] Run it in Vita3K.
- [ ] Capture deterministic test output.
- [ ] Test on physical hardware.
- [ ] Document differences between browser, emulator and hardware results.

### Milestone 4 — engine decisions
- [ ] Decide which systems belong in the platform layer.
- [ ] Decide the first renderer/backend boundary.
- [ ] Document resource ownership and memory budgets.
- [ ] Choose features based on measured benefit, not visual ambition alone.

---

## 8. Source references

### Killzone: Mercenary

- PlayStation Blog, **“Killzone: Mercenary – Behind-the-Scenes Interview, Single Player Details”** (4 February 2013):  
  https://blog.playstation.com/2013/02/04/killzone-mercenary-behind-the-scenes-interview-single-player-details/
- PlayStation Blog, **“Killzone: Mercenary: Cash is King”** (8 March 2013):  
  https://blog.playstation.com/2013/03/08/killzone-mercenary-cash-is-king/
- PocketGamer.biz, **“Guerilla warfare: The making of Killzone: Mercenary”** (16 October 2013):  
  https://www.pocketgamer.biz/guerilla-warfare-the-making-of-killzone-mercenary/

### OpenStrike

- Source repository: https://github.com/pocket-nexus/open-strike
- Root license: https://github.com/pocket-nexus/open-strike/blob/main/LICENSE
- Vita build/runtime notes: https://github.com/pocket-nexus/open-strike/blob/main/crates/openstrike-vita/README.md
- Root README: https://github.com/pocket-nexus/open-strike/blob/main/README.md

### Halo3DVita

- Repository: https://github.com/Wonka15/Halo3DVita
- Architecture: https://github.com/Wonka15/Halo3DVita/blob/main/Documentation/ARCHITECTURE.md
- Roadmap: https://github.com/Wonka15/Halo3DVita/blob/main/Documentation/ROADMAP.md
- Bungie engine lessons: https://github.com/Wonka15/Halo3DVita/blob/main/Documentation/BUNGIE_ENGINE_LESSONS_GDC2015.md

---

## 9. Project commitment

Vita Engine Research Lab will learn from the visual ambition of Killzone: Mercenary and the inspectable engineering practices of OpenStrike while remaining an original, reusable engine.

**We will not claim performance we have not measured, reuse code without checking its license, or use proprietary game content as development material.**

The target is not to recreate either reference. The target is to understand the constraints, make sound engineering choices, and build the strongest original 3D engine we can for the PS Vita.
