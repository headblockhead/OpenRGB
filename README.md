## ![OpenRGB](Documentation/Images/OpenRGB.png "OpenRGB Logo")

[![Nix Flake](https://img.shields.io/badge/NIX%20FLAKE-5277C3.svg?logo=NixOS&logoColor=white)](https://nixos.org) [![xc compatible](https://xcfile.dev/badge.svg)](https://xcfile.dev)

This is a fork of OpenRGB, to add compatibility with the [Slab keyboard](https://github.com/headblockhead/slab), easier building and development, and with little plans for merging upstream. If there is interest in some of my changes being merged, I will add another branch with only the needed commits, and make a MR on [the gitlab](https://gitlab.com/CalcProgrammer1/OpenRGB).

## Want to use?

If you want to try this fork, install [nix](https://nixos.org), and run `nix run github:headblockhead/OpenRGB`.

## Build

This version of OpenRGB uses [nix](https://nixos.org), run `nix develop` for a development environment, and `nix build` to build.

## Tasks

> [!IMPORTANT]
> You must be in the `nix develop` shell for these tasks to work!

### build
Directory: ./build

Builds the OpenRGB application.

```bash
qmake ../OpenRGB.pro
make -j$(nproc)
wrapQtApp ./openrgb
```

### run
Directory: ./build

Runs the OpenRGB application.

```bash
./openrgb
```

### clean

Cleans the build directory.

```bash
rm -rf ./build
mkdir build
```
