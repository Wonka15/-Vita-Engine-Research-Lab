# Development setup

For the complete beginner-friendly setup, use [Getting Started](GETTING_STARTED.md). It covers Windows 11 + WSL2, current VitaSDK installation, the First Light prototype, VPK installation, and separate PCH-1000/PCH-2000 hardware validation.

## Toolchain

- [VitaSDK](https://vitasdk.org/) — compiler, headers, libraries and packaging tools.
- [VitaSDK API documentation](https://docs.vitasdk.org/).
- [VitaSDK samples](https://github.com/vitasdk/samples).
- [vitaGL](https://github.com/Rinnegatamante/vitaGL) — first-mesh bootstrap backend; built separately and subject to its own license and runtime prerequisites.

VitaSDK currently recommends WSL2 for Windows users, while also publishing a native Windows toolchain. Follow the upstream installation guide for the current release and exact commands.

## First verification

1. Install VitaSDK and confirm the SDK environment is configured.
2. Build the official VitaSDK hello-world sample.
3. Build the First Light prototype as described in its [README](../Prototypes/FirstLight/README.md).
4. Launch the VPK in Vita3K only as a smoke test, then test on physical hardware.
5. Record the exact SDK, backend, host OS, firmware, homebrew environment and project commit.

## Hardware policy

- PCH-1000 and PCH-2000 are separate required test targets.
- Never infer Fat results from Slim results or vice versa.
- Emulator results must be labelled and kept separate from physical hardware.
- Do not claim a build or hardware run until it has actually been completed.

## Development expectations

- Keep the first renderer prototype small.
- Prefer clear, commented code over premature abstraction.
- Include build and run instructions with every sample.
- Do not commit proprietary game data, firmware files, console keys, or personal device dumps.
- Label emulator results separately from real-hardware results.
