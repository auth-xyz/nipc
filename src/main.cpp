#include "../headers/libcli.hpp"
#include "../headers/libnix.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

int main(int argc, char* argv[]) {
  CLIParser cli;
  TemplateHandler handler(
    "https://raw.githubusercontent.com/auth-xyz/assets/refs/heads/main/templates/package.nix", 
    "template.nix"
  );

  cli.add_argument("add", "a", false, "Package to add");
  cli.add_argument("remove", "r", false, "Package to remove");
  cli.add_argument("search", "q", false, "Searches for package in nixpkgs");
  cli.add_flag("program", "p", "Given packages will be sent to programs instead");
  cli.parse(argc, argv);

  try {
    handler.downloadTemplate();

    if (!cli.argument("add").empty())
    {
      handler.addPackage(cli.argument("add"));
      handler.save();
    }

    if (!cli.argument("remove").empty())
    {
      handler.removePackage(cli.argument("remove"));
      handler.save();
    }
  }
  catch (const std::exception& err) {
    std::cout << err.what() << std::endl;
  }

  return 0;
}
