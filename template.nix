{ pkgs, ... }:
{
  home.packages = with pkgs; [
    hello
    hello2
    hello3
    hello4
  ];

  home.programs = [
  ];
}
