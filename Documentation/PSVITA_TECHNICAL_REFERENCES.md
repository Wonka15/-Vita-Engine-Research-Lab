# PS Vita Technical Reference Library

**Purpose:** A categorized research directory for Vita Engine Research Lab.  
**Status:** Curated starting points; verify current source revisions, build status and licenses before implementation or reuse.

> Study the best. Respect the creators. Build our own technology. Prove everything on the hardware.

This library separates visual case studies from inspectable engineering projects and official/community development tools. A commercial game's public technical postmortem can teach us about constraints and trade-offs; it does not grant access to proprietary implementation details or assets.

## 1. Commercial game case studies

These games show what teams achieved on Vita hardware. They are visual and production references, not code sources.

### Uncharted: Golden Abyss — high-value developer postmortem

- **Primary reading:** [Sony Bend Studio's postmortem at Game Developer](https://www.gamedeveloper.com/design/postmortem-sony-bend-studio-s-i-uncharted-golden-abyss-i-) (2012).
- **Why it matters:** One of the most detailed public developer accounts of building a Vita rendering engine and content pipeline.
- **Reported techniques and lessons:** deferred rendering, cascaded shadow maps, SSAO, real-time lighting, tone mapping, shader simplification for performance, object fade-out instead of conventional LODs in some cases, layered environment detail, reusable modular building blocks, and a live-update workflow tuned against the final Vita display.
- **What we should investigate:** render-target/memory cost, material feature budgets, geometry layering, instancing, visibility/fade strategies, and artist iteration time.
- **Caution:** The postmortem describes that game's own choices and production context. Treat each technique as a candidate to benchmark, not a prescription.

### Killzone: Mercenary — visual ambition and optimisation

- **Developer interviews:** [PlayStation Blog: behind-the-scenes interview](https://blog.playstation.com/2013/02/04/killzone-mercenary-behind-the-scenes-interview-single-player-details/) and [PocketGamer production interview](https://www.pocketgamer.biz/guerilla-warfare-the-making-of-killzone-mercenary/).
- **Existing project study:** [Killzone: Mercenary and OpenStrike reference study](REFERENCE_ENGINES.md).
- **Why it matters:** An ambitious Vita visual target for materials, smoke/fog, particles, reflections, shadows, texture use and level visibility.
- **What we should investigate:** lighting-aware particles, reflection approximations, material response, visibility and LOD, texture memory, overdraw, and frame-time stability.
- **Caution:** Public interviews do not reveal the complete proprietary renderer. Do not infer an exact implementation from a visual effect.

### WipEout 2048 — fast motion, lighting and image quality

- **Developer interview:** [Push Square interview with Sony Studio Liverpool](https://www.pushsquare.com/news/2012/01/interview_sony_studios_liverpool_wipeout_2048).
- **Why it matters:** A useful reference for high-speed scenes, track readability, lighting, effects and maintaining visual clarity under motion.
- **What we should investigate:** material/shader cost, environment lighting approximations, bloom/tone mapping, effects budgeting and frame pacing.
- **Caution:** Some detailed rendering claims found in secondary summaries need stronger primary-source confirmation. Record unverified details as hypotheses.

### Gravity Rush — stylised rendering and PhyreEngine context

- **Official interview:** [PlayStation Blog interview with director Keiichiro Toyama and art director Yoshiaki Yamaguchi](https://blog.playstation.com/?p=71888).
- **Engine context:** [PhyreEngine](https://en.wikipedia.org/wiki/PhyreEngine) is Sony's proprietary multi-platform engine technology and is reported in public secondary sources as the engine used for Gravity Rush.
- **Why it matters:** Strong art direction, stylised lighting, readable silhouettes, camera composition and visual tricks that create a rich image within a handheld budget.
- **What we should investigate:** art direction as a performance tool, fake/cheap reflections, selective detail, camera-aware visibility and consistent frame pacing.
- **Caution:** PhyreEngine is proprietary, not an unrestricted open-source engine. Public information is research material, not a license to use Sony source, tools or assets. Treat specific Gravity Rush implementation claims as secondary-source reports unless independently verified.

## 2. Bungie engine lineage research

The [Blam! Engine Lineage](BLAM_ENGINE_LINEAGE.md) document tracks Halo: Combat Evolved, Halo 2, Halo 3, Halo 3: ODST and Halo: Reach, along with Bungie's internal Blam!-based experiments, the Reach-to-Tiger transition, later descendants and important distinctions such as Halo Wars' separate Phoenix engine. It is a historical architecture study, not a source or asset collection.

## 3. Native Vita graphics APIs and development foundations

### VitaSDK — official community toolchain and API documentation

- [VitaSDK website](https://vitasdk.org/)
- [API documentation](https://docs.vitasdk.org/)
- [Samples repository](https://github.com/vitasdk/samples)
- **Study:** CMake projects, system APIs, GXM examples, SELF/VPK packaging and the smallest working native application.
- **Use in our project:** Establish a reproducible toolchain and a minimal native test before designing the full engine.
- **Note:** VitaSDK is a community homebrew development project, not an official Sony SDK.

### vitaGL — OpenGL-style wrapper over GXM

- [Source repository](https://github.com/Rinnegatamante/vitaGL)
- **What it is:** An open-source OpenGL wrapper that translates a supported subset of calls to the Vita's SceGxm graphics API.
- **Study:** Sample projects for meshes, lighting, fog, environment maps/skyboxes, occlusion queries, deferred rendering, SSAO, buffers and texture formats.
- **Use in our project:** Benchmark development speed, supported feature coverage and overhead against a small direct-GXM prototype.
- **License warning:** The repository identifies GPL/LGPL licensing files. Review the exact library version, files and license obligations before linking, modifying or distributing it. Also account for its documented shader-compiler prerequisite. Do not assume the wrapper is a frictionless drop-in.

## 4. Open-source and community Vita engineering projects

These are inspectable engineering references. Inspect current source and licenses at the exact revision before borrowing anything.

### OpenStrike — native Vita application and platform architecture

- [GitHub repository](https://github.com/pocket-nexus/open-strike)
- **Study:** Vita host, GXM integration, shared simulation, Pocket3D, build scripts, resource lifecycle and Vita3K automated tests.
- **Project notes:** [Our OpenStrike reference study](REFERENCE_ENGINES.md).
- **License:** Root repository is MIT, but submodules and dependencies may have separate terms. Review each component independently.

### Godot 4 PS Vita port — modern engine adaptation

- [GitHub repository](https://github.com/WolffsRoom/godot4-psvita)
- **Why it matters:** A substantial community effort adapting a modern engine to Vita, including native GXM rendering paths and a Windows export workflow.
- **Study:** Platform integration, loader/runtime split, renderer boundaries, draw batching, texture budgets, CPU/GPU overlap, diagnostics and real-device validation.
- **Scope warning:** The project reports both 2D and experimental 3D functionality; verify the current state and limitations in upstream documentation. It is not an official Godot release.

### repentogxm — static recompilation and Vita performance study

- [Project repository](https://qgithub.com/0xl0cal/repentogxm)
- **Why it matters:** A modern example to investigate for porting workflow, native ARM build, graphics translation and performance measurement.
- **Study:** Recompilation pipeline, graphics API use, CPU/GPU bottlenecks, compatibility layers and test methodology.
- **Caution:** Confirm repository availability, current status and exact licenses before relying on it. It is not a general-purpose engine template.

### Live2D Cubism 4 Vita — native GXM framework port

- [Project overview](https://www.gamebrew.org/wiki/Live2D_Cubism_4_Vita)
- **Why it matters:** A framework-porting example for animation, CPU optimisation and native Vita graphics integration.
- **Study:** Framework adaptation, NEON optimisation, model update/render separation and resolution/antialiasing trade-offs.
- **Scope warning:** Primarily a 2D/character animation reference, not a general 3D renderer. Check upstream project and component licenses.

## 5. Community directories and further discovery

### PS Vita Porting Wiki

- [PS Vita Porting Wiki](https://metalsyntax.github.io/psvita-porting-wiki/)
- **Use:** Discover practical porting notes, libraries, graphics approaches and platform-specific pitfalls.
- **Caution:** A community wiki is a discovery aid, not final authority. Verify every technical claim against source code, documentation or a reproducible test.

### VitaHarbor project atlas

- [VitaHarbor](https://vitaharbor.vercel.app/)
- **Use:** Find current and historical Vita ports, homebrew, wrappers and development projects.
- **Caution:** Independently verify project activity, upstream source, provenance and license. A directory listing is not proof of quality or permission to reuse.

## 6. How we will turn references into engineering decisions

Every useful reference should lead to a small, independent experiment:

| Research area | First Research Lab experiment | Evidence to record |
|---|---|---|
| Geometry and visibility | Mesh-count and occlusion scene | Visible/submitted triangles, draw calls, frame time |
| Materials and lighting | Small material gallery | Shader variants, CPU/GPU time, image captures |
| Shadows | Baked/contact shadow baseline, then shadow-map prototype | Render-target memory, frame time, artifacts |
| Reflections | Static environment map before dynamic approaches | Memory, update cost, visual stability |
| Fog and particles | Layered fog cards and lit/unlit particle comparison | Overdraw, particle count, frame time |
| Asset pipeline | Reusable modular scene pieces and texture variants | Package size, load time, memory footprint |
| Runtime architecture | Minimal native GXM and vitaGL comparison | Build complexity, API coverage, frame time |
| Validation | Identical scene across browser, Vita3K and hardware | Separate results, build revision and test conditions |

### Validation labels

- **Documented:** stated by a credible source or upstream project.
- **Source-confirmed:** verified in the relevant source code at a recorded revision.
- **Emulator-tested:** run in Vita3K; not a hardware performance claim.
- **Hardware-tested:** run on a physical PS Vita with conditions recorded.
- **Hypothesis:** an idea to test, not an established fact.

Never compare browser FPS directly with Vita FPS. Browser lab metrics are useful for interaction and visual iteration only. Vita3K is useful for launch/runtime regression tests. Physical Vita measurements are required for hardware claims.

## 7. Licensing and intellectual-property policy

- Commercial game references are for learning from public information. Do not extract or commit proprietary source, shaders, tools, textures, models, maps, audio or other game data.
- Open-source code is not automatically reusable merely because a project is public. Check the exact file, revision, license, submodules, dependencies and required notices.
- Keep a provenance record for every third-party component actually incorporated: origin URL, revision, author, license, modifications and obligations.
- The repository's MIT license applies only to original project material that the repository authors have rights to license. It does not relicense third-party work or grant rights to game IP.
- Prefer independent implementations of useful ideas. When code reuse is genuinely needed, preserve all required notices and document the decision.

## 8. Recommended reading order

1. Read the [Uncharted: Golden Abyss postmortem](https://www.gamedeveloper.com/design/postmortem-sony-bend-studio-s-i-uncharted-golden-abyss-i-) for detailed Vita production and rendering trade-offs.
2. Read [VitaSDK getting started](https://vitasdk.org/) and build a clean sample.
3. Compare [vitaGL samples](https://github.com/Rinnegatamante/vitaGL) with direct GXM examples.
4. Read the [Godot Vita port](https://github.com/WolffsRoom/godot4-psvita) for modern engine/platform integration.
5. Study [OpenStrike](https://github.com/pocket-nexus/open-strike) for its native app workflow and tests.
6. Use Killzone, WipEout and Gravity Rush as visual case studies; convert each desired effect into a measurable original test.

## 9. Relationship to the rest of this repository

- [Architecture](ARCHITECTURE.md)
- [Roadmap](ROADMAP.md)
- [Development setup](DEVELOPMENT.md)
- [Bungie engine architecture lessons](BUNGIE_ENGINE_LESSONS_GDC2015.md)
- [Killzone: Mercenary and OpenStrike reference study](REFERENCE_ENGINES.md)
- [Browser graphics laboratory source](../docs/index.html)
- [Project README](../README.md)
