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

  /*cli.add_flag("verbose", "v", "Enable verbose output");*/
  /*cli.add_argument("input", "i", true, "Input file");*/
  /*cli.add_argument("output", "o", false, "Output file");*/
  /*cli.parse(argc, argv);*/
  /**/
  /*if (cli.flag("verbose")) {*/
  /*    std::cout << "Verbose mode enabled\n";*/
  /*}*/
  /**/
  /*std::cout << "Input file: " << cli.argument("input") << '\n';*/
  /*if (!cli.argument("output").empty()) {*/
  /*    std::cout << "Output file: " << cli.argument("output") << '\n';*/
  /*}*/
  /**/
  return 0;
}
