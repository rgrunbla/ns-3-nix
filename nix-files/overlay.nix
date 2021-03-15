self: super:


let
  lib = self.pkgs.lib;
  stdenv = self.pkgs.stdenv;
  pythonEnv = super.python3;
in
{
  ns-3 = super.ns-3.override {
    build_profile = "optimized";
    modules = [ "all_modules" "core" "network" "internet" "point-to-point" "point-to-point-layout" "fd-net-device" "netanim" "flow-monitor" "mobility" ];
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
      src = super.fetchFromGitHub
        {
          owner = "rgrunbla";
          repo = "ns-3-33";
          rev = "main";
          sha256 = "089aq2bfn2q3m0hcapbn8ys3p3wqrl18a4zvp0nk1q8lp5rik11k";
        };
      patches = [ ];
      doCheck = false;
      checkPhase = ''
        ${pythonEnv.interpreter} ./test.py --nowaf --verbose
      '';
    });
}
