#!/usr/bin/env nix-shell
with import <nixpkgs> { overlays = [ (import ./nix-files/overlay.nix) ]; };

mkShell {
  nativeBuildInputs = [
    llvmPackages_latest.clang
    pkg-config
    valgrind
  ];
  buildInputs = [ my-ns-3 ];
}