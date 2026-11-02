# Contributing

Thanks for helping make Vita Engine Research Lab a practical, hardware-first graphics project.

## Current focus

The immediate milestone is **M0 — First Light**: build and run a small native Vita application on both PCH-1000 and PCH-2000 hardware, then capture repeatable timing evidence.

## Before opening a pull request

1. Explain the problem or experiment and the expected result.
2. Keep changes small and focused.
3. Include exact build steps and toolchain/library versions.
4. Separate source-confirmed facts, emulator tests, and physical-hardware results.
5. Never claim a hardware test unless the exact console model was used.
6. Do not add proprietary game assets, extracted game data, firmware modules, console keys, or copyrighted binaries.
7. Check third-party licenses before adding code or linking dependencies.

## Hardware test reports

When testing on a Vita, include:
- Model: PCH-1000 or PCH-2000.
- Firmware and homebrew environment.
- VitaSDK, backend/library and project commit.
- Build configuration and resolution.
- Test scene and duration.
- Average FPS, frame time (ms), memory readings and observed issues.
- Photo/video evidence where possible, with personal identifiers and network details removed.

## Code conventions

- Prefer readable C/C++ and explicit ownership/lifetime.
- Keep platform calls under `Platform/Vita` or a backend.
- Avoid premature abstractions before the first working render path.
- Preserve a buildable baseline; document anything that currently cannot be built.

## Good first contributions

Start with the open issues labelled `good first issue`:

- [Test First Light on PCH-1000](https://github.com/Wonka15/Vita-Engine-Research-Lab/issues/1)
- [Test First Light on PCH-2000](https://github.com/Wonka15/Vita-Engine-Research-Lab/issues/2)
- [Prototype a direct GXM comparison](https://github.com/Wonka15/Vita-Engine-Research-Lab/issues/3)
- [Add main RAM / VRAM diagnostics](https://github.com/Wonka15/Vita-Engine-Research-Lab/issues/4)

More good-first tasks can be added as the hardware baseline becomes stable.
