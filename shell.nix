#!/usr/bin/env nix-shell
with import <nixpkgs> { overlays = [ (import ./nix-files/overlay.nix) ]; };
stdenv.mkDerivation {
  name = "ns-3";
  buildInput = [ my-ns-3 ];
}