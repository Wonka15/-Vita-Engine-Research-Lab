---
layout: default
title: Licensing and distribution
permalink: /licensing.html
---
# Licensing and distribution

The repository's MIT license applies to original project code authored for Vita Engine Research Lab. It does **not** automatically relicense third-party libraries, SDK components, game assets, firmware, or console software.

## vitaGL

vitaGL is a separate third-party project. Before redistributing a VPK that links or bundles vitaGL, inspect the exact version's repository license and any notices or attribution requirements. Preserve required copyright and license notices in source distributions and include applicable notices with binary releases.

## VitaSDK

VitaSDK is a toolchain and collection of SDK components. Check the license and redistribution terms for every component that is linked, copied, or packaged. Do not assume that installing or building with the toolchain grants permission to redistribute every SDK file.

## Release checklist

- Record exact dependency names, versions, source URLs and licenses.
- Distinguish dynamically linked, statically linked and bundled components.
- Preserve copyright notices and include required license texts.
- Do not package Sony firmware, proprietary system modules, console keys, or extracted commercial-game data.
- Review the current upstream license files before each release; licenses and dependency composition can change.
- If a dependency's terms are unclear, do not distribute the combined VPK until the issue is resolved.

This page is engineering guidance, not legal advice. A release should be reviewed against the exact source and binary contents being distributed.
