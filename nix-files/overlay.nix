self: super:


let
  lib = self.pkgs.lib;
  stdenv = self.pkgs.stdenv;
  pythonEnv = super.python3;
in
{
  ns-3 = super.ns-3.override {
    build_profile = "debug";
    modules =  ["all_modules" "core" "network" "internet" "point-to-point" "point-to-point-layout" "fd-net-device" "netanim" "flow-monitor" "mobility"];
    stdenv = super.ccacheStdenv.override {
      extraConfig = ''
        export CCACHE_LOGFILE=/var/cache/ccache/ccache.log
        export CCACHE_DIR=/var/cache/ccache
        export CCACHE_UMASK=007
      '';
    };
  };

  my-ns-3 = self.ns-3.overrideAttrs
    (oldAttrs: rec {
      src = /home/remy/ns-3/ns-3;
      patches = [ ];
      doCheck = false;
      checkPhase = ''
        ${pythonEnv.interpreter} ./test.py --nowaf --verbose
      '';
    });
}
