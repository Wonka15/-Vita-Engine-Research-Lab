# Getting Started — native PS Vita development

This guide builds the repository's first native prototype. It is written for Windows 11 users using WSL2; native Windows VitaSDK builds are also available, but the commands below assume the Linux shell recommended by VitaSDK.

## 1. Install WSL2 and VitaSDK

Install WSL2 with a supported Ubuntu distribution, open its terminal, then follow the current [VitaSDK getting-started guide](https://vitasdk.org/).

The current upstream bootstrap flow is:

```sh
sudo apt update
sudo apt install -y git cmake make python3 build-essential
export VITASDK=/usr/local/vitasdk
export PATH="$VITASDK/bin:$PATH"
git clone https://github.com/vitasdk/vdpm
cd vdpm
./bootstrap-vitasdk.sh
vdpm status
```

Add these exports to `~/.bashrc` so they persist:

```sh
export VITASDK=/usr/local/vitasdk
export PATH="$VITASDK/bin:$PATH"
```

VitaSDK versions and package channels change. Check the official site before installing; do not assume a tutorial's pinned release is still current.

## 2. Get the repository

```sh
git clone https://github.com/Wonka15/-Vita-Engine-Research-Lab.git
cd -Vita-Engine-Research-Lab
```

If Git Bash or your shell interprets the leading dash as an option, use the full path or `cd -- -Vita-Engine-Research-Lab`.

## 3. Build the First Light prototype

The first 3D mesh uses vitaGL, a GXM-backed OpenGL subset. Build/install vitaGL separately according to its upstream README, then build the project:

```sh
cd Prototypes/FirstLight
make clean
make
```

The output should be `first_light.vpk`. This step has not yet been verified in this repository's CI or on the owner's consoles; report any compiler/linker issue with the full error and SDK/library versions.

**vitaGL runtime requirement:** its upstream documentation requires the console's `libshacccg.suprx` shader compiler module. Follow the upstream extraction instructions on your own console. Do not download or share firmware modules, keys, or decrypted system files.

## 4. Install and run on the console

1. Enable your existing homebrew environment and open VitaShell.
2. Press SELECT to start VitaShell's FTP server.
3. Upload `first_light.vpk` to the Vita.
4. Install the VPK from VitaShell and launch **Vita Engine First Light**.
5. Confirm that the cube rotates and the FPS/frame-time overlay appears.
6. Press START to exit.

## 5. Test both hardware revisions

Run the same VPK/build and the same benchmark procedure on:
- PCH-1000 (original OLED model).
- PCH-2000 (Slim LCD model).

Record separate results. Do not infer one model's results from the other or substitute Vita3K results for hardware results.

## 6. Evidence labels

- **Build-tested:** compiler and linker produced the expected VPK.
- **Emulator-tested:** the VPK launches in Vita3K.
- **Hardware-tested — PCH-1000:** run confirmed on an original Vita.
- **Hardware-tested — PCH-2000:** run confirmed on a Slim Vita.

Only the last two labels prove the corresponding hardware path.
