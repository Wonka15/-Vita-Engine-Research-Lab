# First Light Backend Comparison — Results Log

**Report date:** 2026-11-01  
**Repository:** [Vita Engine Research Lab](https://github.com/Wonka15/-Vita-Engine-Research-Lab)  
**Status:** Preliminary build investigation; no valid head-to-head 3D benchmark yet.

## Objective

Compare the same original rotating six-face cube through two Vita graphics paths:

1. **vitaGL control:** the existing First Light prototype, which uses an OpenGL-style API translated to SceGxm.
2. **Direct GXM candidate:** a native SceGxm renderer using GXM shader programs and `sceGxmDraw`.

A fair comparison must hold scene geometry, colours, camera, 960×544 output, depth format, HUD workload, animation speed, frame pacing, warm-up and capture duration constant.

## Confirmed repository state

| Item | Current evidence | Result |
|---|---|---|
| vitaGL First Light source | Existing rotating six-colour cube, depth test and live FPS/frame-time HUD | Present |
| vitaGL VPK packaging | GitHub Actions run 41373869565 reached and completed the vitaGL CMake/VPK stage | Build stage succeeded |
| Direct GXM source | Direct SceGxm initialization, CDRAM display buffers, colour surfaces, sync objects and display queue | Bring-up only |
| Direct GXM matching cube | No shader-backed cube implementation yet | Not implemented |
| Direct GXM VPK | The run failed during direct-GXM compilation before artifact upload | Not produced by that run |
| Physical PS Vita validation | No device run recorded | Not tested |
| Vita3K validation | No emulator run recorded | Not tested |

## Build investigation

GitHub Actions run [41373869565](https://github.com/Wonka15/-Vita-Engine-Research-Lab/actions/runs/41373869565) failed in the Direct GXM target.

- The vitaGL library compiled and installed in the CI container.
- The existing First Light vitaGL target compiled and generated its VPK.
- Direct GXM compilation stopped because `sceKernelCpuDcacheWritebackRange` was not declared by the VitaSDK headers in the selected toolchain.
- The attempted header include did not resolve that API mismatch.
- The source has now been adjusted to remove that unavailable call and follow the public VitaSDK framebuffer sample's direct CDRAM update pattern. This is a corrective source change, **not yet a confirmed successful rebuild**.

## Why there are no FPS results

The Direct GXM prototype currently draws a CPU-generated diagnostic framebuffer pattern. It does not submit the same 3D cube to the GPU. Comparing its frame rate with the vitaGL cube would mix different workloads and produce a misleading result.

A real direct-GXM cube requires a complete GXM rendering setup: context and ring buffers, render target, depth surface, shader patcher, registered vertex/fragment programs, vertex/index data, uniforms or transformed vertices, scene submission and synchronized presentation. The VitaSDK documents `sceGxmDraw` and the shader-patcher API; a framebuffer/display-queue sample alone is not a 3D renderer.

## Benchmark protocol once both cube renderers exist

1. Use the same physical Vita, firmware, power state and screen brightness.
2. Use 960×544, the same colour/depth formats, same cube vertices/indices, colours, camera and animation.
3. Keep the HUD identical or disable it in both measured runs; do not measure different HUD workloads.
4. Use the same frame-pacing/vsync policy.
5. Warm up for 10 seconds, then capture 120 seconds per run.
6. Run each backend at least three times, alternating order to reduce thermal/order bias.
7. Record average FPS, average CPU frame time, median and 99th-percentile CPU frame time, 1% low FPS, draw calls, memory allocations and visual parity.
8. Label CPU-side timing honestly. Do not call it GPU time. Add GPU timing only if a valid hardware measurement method is available.
9. Repeat separately on PCH-1000 and PCH-2000 if both are available.

## Current conclusion

**No performance winner can be named.** The only confirmed progress is that the vitaGL baseline builds and the direct-GXM bring-up source exists. The previous CI failure is isolated to the direct-GXM compile stage. The next engineering milestone is a shader-backed direct-GXM cube, then a repeatable shared benchmark harness. Physical hardware results remain pending.

## Technical references

- [VitaSDK GXM API — sceGxmDraw and scene submission](https://docs.vitasdk.org/gxm_8h.html)
- [VitaSDK public framebuffer/display-queue sample](https://github.com/vitasdk/samples/blob/master/ime/src/main.c)
- [Vita3D_Sample_cpp — independent direct-GXM shader/context example](https://github.com/FirebirdTA01/Vita3D_Sample_cpp)
- [VitaSDK](https://vitasdk.org/)

The external sample is a study reference only. No third-party source or shader binaries are copied into this project by this report.
