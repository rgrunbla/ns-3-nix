#!/usr/bin/env nix-shell
with import <nixpkgs> { overlays = [ (import ./nix-files/overlay.nix) ]; };


let
  my-python-packages = python-packages: with python-packages; [
    pyyaml
  ]; 
  python-with-my-packages = python3.withPackages my-python-packages;
in
mkShell {
  nativeBuildInputs = [
    llvmPackages_latest.clang
    pkg-config
    valgrind
    python-with-my-packages
  ];
  buildInputs = [ my-ns-3 ];
}
