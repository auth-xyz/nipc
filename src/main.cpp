#include "../headers/libcli.hpp"
#include "../headers/libnix.hpp"

#include "../headers/nipSearch.hpp" 


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

        if (!cli.argument("add").empty()) {
            handler.addPackage(cli.argument("add"));
            handler.save();
        }

        if (!cli.argument("remove").empty()) {
            handler.removePackage(cli.argument("remove"));
            handler.save();
        }

        if (!cli.argument("search").empty()) {
            std::string search_query = cli.argument("search");
            
            // Create an instance of NixAPI and check if the site is up
            NixAPI nixApi;
            if (nixApi.is_site_up()) {
                std::cout << "Searching for package: " << search_query << std::endl;

                std::vector<std::string> results = nixApi.search_packages(search_query);
                if (results.empty()) {
                    std::cout << "No packages found for: " << search_query << std::endl;
                } else {
                    std::cout << "Packages found:\n";
                    for (const auto& pkg : results) {
                        std::cout << pkg << std::endl;
                    }
                }
            } else {
                std::cout << "Nix package search website is down. Please try again later." << std::endl;
            }
        }
    }
    catch (const std::exception& err) {
        std::cout << err.what() << std::endl;
    }

    return 0;
}

