# First Light — native Vita graphics prototype

**Target:** PlayStation Vita PCH-1000 (original/Fat) and PCH-2000 (Slim)  
**Status:** Alpha VPK build has succeeded in CI; the latest HUD/source polish is being rebuilt. Physical-device verification is still pending.

This is the first native graphics milestone. It draws an original rotating cube using vitaGL, which translates a supported OpenGL subset to the Vita's SceGxm GPU interface. It is a native Vita homebrew application, not a browser demo or emulator.

## Why vitaGL for the first mesh?

The current official VitaSDK samples focus on the general application/toolchain workflow; the older standalone GXM examples are no longer present in the current samples tree. vitaGL provides a practical route to a first GPU-rendered mesh while keeping the application native to Vita hardware.

This is a bootstrap choice, not a final backend decision. The decision log defines a fair comparison against a direct GXM renderer. The prototype must not be described as a direct-GXM implementation.

## What it currently aims to show

- A rotating indexed-style cube mesh (six independently coloured faces).
- Depth testing and perspective projection.
- A rounded benchmark badge with a cyan speedometer icon.
- Live FPS and most-recent-frame-time readout.
- Beginner-friendly source comments, section labels, and consistent formatting.
- Start button exits the application.
- The same build should run on PCH-1000 and PCH-2000; record each device separately.

## Prerequisites

1. Install a current VitaSDK release and add `$VITASDK/bin` to `PATH`.
2. Install/build vitaGL from its upstream repository using its documented `make install` process.
3. vitaGL requires the console's `libshacccg.suprx` shader compiler module to be extracted/decrypted on the Vita. Follow the upstream vitaGL documentation; do not download or redistribute Sony firmware modules.
4. Use a homebrew-enabled Vita with VitaShell.

The prototype does not include VitaSDK, vitaGL, Sony modules, firmware, keys, or any proprietary assets.

## Build

From this directory, in the VitaSDK shell / WSL2 environment:

```sh
export VITASDK=/usr/local/vitasdk
export PATH="$VITASDK/bin:$PATH"
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE="$VITASDK/share/vita.toolchain.cmake"
cmake --build build --parallel
```

Expected output: `build/first_light.vpk`. The GitHub Actions workflow also builds the package and retains it as a 30-day workflow artifact when the build succeeds. The artifact is a test build, not yet a public release.

If the linker cannot find `-lvitaGL`, confirm vitaGL was installed into the active VitaSDK prefix and that the SDK and vitaGL were built for the same toolchain.

## Install on a Vita

1. Connect the Vita and open VitaShell.
2. Press **SELECT** to start VitaShell's FTP server.
3. From the PC, upload `first_light.vpk` to the Vita (for example, with an FTP client).
4. In VitaShell, open the uploaded VPK and install it.
5. Launch **Vita Engine First Light** from LiveArea.
6. Press **START** to exit.

Do not publish a hardware-success claim until this has been repeated on a physical PCH-1000 and a physical PCH-2000.

## Benchmark protocol

For each console, record:
- Exact model (PCH-1000 or PCH-2000), firmware, and homebrew environment.
- VitaSDK and vitaGL revisions.
- Build commit and whether the app was rebuilt from clean.
- Average FPS and frame time, plus observed minimum/maximum or a captured frame-time log.
- Test duration, power mode, temperature/battery state if known, and any crashes or visual issues.

Use the shared [First Light scene definition](../../Tests/SCENES/FIRST_LIGHT.md) and [results template](../../Tests/RESULTS_TEMPLATE.md). Never merge Fat and Slim results into one unlabeled number.

## Licensing note

The prototype source is original project code. vitaGL is a separate third-party dependency with its own GPL/LGPL licensing files and obligations. Review the exact vitaGL revision, linked components, and distribution obligations before distributing a combined VPK. This repository does not vendor or relicense vitaGL.
