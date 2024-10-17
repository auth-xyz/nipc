#include "../headers/libcli.hpp"
#include "../headers/libnix.hpp"

#include <iostream>

int main(int argc, char* argv[]) {
  CLIParser cli;
  TemplateHandler handler(
    "https://raw.githubusercontent.com/auth-xyz/assets/refs/heads/main/templates/package.nix", 
    "template.nix"
  );

  cli.add_argument("install", "i", false, "Package to install");
  cli.add_argument("remove", "r", false, "Package to remove");
  cli.add_argument("search", "q", false, "Searches for package in nixpkgs");
  cli.add_flag("program", "p", "Given packages will be sent to programs instead");
  cli.parse(argc, argv);

  try {
    handler.downloadTemplate();

    if (!cli.argument("install").empty())
    {
      handler.addPackage(cli.argument("install"));
      handler.save();
    }

    if (!cli.argument("remove").empty())
    {
      handler.removePackage(cli.argument("remove"));
      handler.save();
    }
  }
  catch (const std::exception&) {
  }

  return 0;
}
