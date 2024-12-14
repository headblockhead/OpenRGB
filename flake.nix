{
  description = "Open source RGB lighting control that doesn't depend on manufacturer software.";
  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };
  outputs = { nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem
      (system:
        let
          pkgs = import nixpkgs { inherit system; };
        in
        rec {
          devShells.default = pkgs.mkShell {
            buildInputs = with pkgs; with pkgs.libsForQt5.qt5; [
              xc

              coreutils
              qmake
              pkg-config
              wrapQtAppsHook
              libusb1
              hidapi
              mbedtls_2
              qtbase
              qttools
            ];
          };
          packages.OpenRGB = pkgs.stdenv.mkDerivation {
            pname = "openrgb";
            version = "v1";
            src = ./.;

            nativeBuildInputs = with pkgs; with pkgs.libsForQt5.qt5; [ qmake pkg-config wrapQtAppsHook ];
            buildInputs = with pkgs; with pkgs.libsForQt5.qt5; [ libusb1 hidapi mbedtls_2 qtbase qttools ];

            postPatch = ''
              patchShebangs scripts/build-udev-rules.sh
              substituteInPlace scripts/build-udev-rules.sh \
                --replace /bin/chmod "${pkgs.coreutils}/bin/chmod"
              substituteInPlace scripts/build-udev-rules.sh \
                --replace /usr/bin/env "${pkgs.coreutils}/bin/env"
            '';

            doInstallCheck = true;
            installCheckPhase = ''
              HOME=$TMPDIR $out/bin/openrgb --help > /dev/null
            '';
          };
          packages.default = packages.OpenRGB;
        }
      );
}
