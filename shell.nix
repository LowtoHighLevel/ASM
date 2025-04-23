let
  pkgs = import <nixpkgs> { };
in
pkgs.mkShell {
  nativeBuildInputs = with pkgs; [
    git
    zig
    gcc
    lazygit
    valgrind
    gnumake
    gdb
  ];
}
