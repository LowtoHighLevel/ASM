let
  pkgs = import <nixpkgs> { };
in
pkgs.mkShell {
  nativeBuildInputs = with pkgs; [
    git
    clang
    lazygit
    valgrind
    gnumake
    gdb
  ];
}
