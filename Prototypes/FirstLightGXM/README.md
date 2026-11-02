# First Light — Direct GXM bring-up

This separate VPK is the direct SceGxm path, independent of vitaGL. It initializes SceGxm, allocates two CDRAM framebuffers, creates GXM color surfaces and synchronization objects, and presents an animated diagnostic pattern through the GXM display queue.

**Scope warning:** this is a direct-GXM display/presentation bring-up, not yet the matching 3D cube benchmark. Its frame rate must not be compared with the vitaGL cube as a backend performance result. The next step is a shader-backed cube using `sceGxmDraw`, then both builds can share the same scene and VPR measurement boundaries.

## Build

Use VitaSDK (the GitHub Actions workflow builds this target automatically):

```sh
export VITASDK=/usr/local/vitasdk
export PATH="$VITASDK/bin:$PATH"
make clean
make
```

Output: `first_light_gxm.vpk`.

## Controls

Press **START** to exit.

## Technical basis

The framebuffer, GXM color-surface, sync-object and display-queue sequence is adapted from the public VitaSDK `samples/ime/src/main.c` sample. No Sony firmware files, proprietary assets, or third-party rendering library are bundled.

## Validation

A successful CI build proves compilation and VPK packaging only. Test on a homebrew-enabled physical Vita before treating runtime behavior as confirmed. Record model, firmware and environment.
