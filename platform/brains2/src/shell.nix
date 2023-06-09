{pkgs ? import <nixpkgs> {}}:
  pkgs.mkShell {
    nativeBuildInputs = [
      pkgs.gcc-arm-embedded-12
    ];
  }
