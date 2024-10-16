{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  buildInputs = with pkgs; [
    gcc
    gnumake
    cmake
    curl
    curl.dev
  ];

  shellHook = ''echo "[god i hate my life]"'';
}
