#!/usr/bin/env bash

nix-shell --command exit
nix-store -qR --include-outputs $(nix-instantiate shell.nix) | cachix push custom-ns-3-build

