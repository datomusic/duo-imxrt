{pkgs ? import <nixpkgs> {}}:
  pkgs.mkShell {
    shellHook = ''
      export TOOLCHAIN_FILE=$(pwd)/../armgcc/global-arm-gcc.cmake 
    '';
    nativeBuildInputs = [
      pkgs.gcc-arm-embedded
    ];
  }
