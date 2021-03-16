#!/usr/bin/env nix-shell
with import <nixpkgs> { overlays = [ (import ./nix-files/overlay.nix) ]; };

let
  my-python-packages = python-packages: with python-packages; [
    numpy
    plotly
    matplotlib
    pandas
    notebook
    psutil
  ]; 
  python-with-my-packages = python3.withPackages my-python-packages;
in
mkShell {
  nativeBuildInputs = [
    python-with-my-packages
  ];
}
