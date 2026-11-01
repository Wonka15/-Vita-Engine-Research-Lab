# Development setup

## Toolchain

Use [VitaSDK](https://vitasdk.org/) and follow its current installation guide. The official VitaSDK getting-started instructions currently recommend WSL2 for Windows users, while also publishing a native Windows toolchain. Check the upstream instructions for the supported release and exact commands before installing.

Useful upstream references:

- [VitaSDK getting started](https://vitasdk.org/)
- [VitaSDK API documentation](https://docs.vitasdk.org/)
- [VitaSDK samples](https://github.com/vitasdk/samples)
- [VitaSDK graphics API](https://docs.vitasdk.org/group__Graphics.html)
- [VitaSDK GXM documentation](https://docs.vitasdk.org/group__SceGxm.html)

## First verification

1. Install VitaSDK using its current official instructions.
2. Confirm the SDK environment is configured.
3. Build the official hello-world sample using the sample's current CMake instructions.
4. Confirm that the generated application runs in a compatible emulator or on a Vita configured for homebrew.
5. Record the exact SDK release, host OS, and test environment in your notes.

## Windows note

For Windows, follow VitaSDK's current WSL2 or native Windows instructions. Do not copy old install commands from unrelated tutorials without checking that they match the currently supported SDK release.

## Development expectations

- Keep the first renderer prototype small.
- Prefer clear, commented code over premature abstraction.
- Include build and run instructions with every sample.
- Do not commit proprietary game data, firmware files, console keys, or personal device dumps.
- Label emulator results separately from real-hardware results.
