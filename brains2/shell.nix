{pkgs ? import <nixpkgs> {}}:
  pkgs.mkShell {
    shellHook = ''
      export TOOLCHAIN_FILE=$(pwd)/sdk/armgcc/global-arm-gcc.cmake
    '';
    nativeBuildInputs = [
      pkgs.gcc-arm-embedded
    ];
  }
