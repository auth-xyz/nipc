#include <algorithm>
#include <memory>
#include "../headers/libnix.hpp"
#include "../headers/nipUtils.hpp"

TemplateHandler::TemplateHandler(const std::string& url, const std::string& localFile)
    : url(url), localFile(localFile) {
    loadTemplate();
}

bool TemplateHandler::downloadTemplate() {
    try {
        std::string content = fetch_url(url);
        save_to_file(localFile, content);
        return true;
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << '\n';
        return false;
    }
}

void TemplateHandler::addPackage(const std::string& pkg) {
    packages.insert(std::lower_bound(packages.begin(), packages.end(), pkg), pkg);
}

void TemplateHandler::addProgram(const std::string& prog) {
    programs.insert(std::lower_bound(programs.begin(), programs.end(), prog), prog);
}

void TemplateHandler::removePackage(const std::string& pkg) {
    packages.erase(std::remove(packages.begin(), packages.end(), pkg), packages.end());
}

void TemplateHandler::removeProgram(const std::string& prog) {
    programs.erase(std::remove(programs.begin(), programs.end(), prog), programs.end());
}

void TemplateHandler::listPackages() const {
    std::cout << "Packages:\n";
    for (const auto& pkg : packages) {
        std::cout << pkg << '\n';
    }
}

void TemplateHandler::listPrograms() const {
    std::cout << "Programs:\n";
    for (const auto& prog : programs) {
        std::cout << prog << '\n';
    }
}

bool TemplateHandler::save() const {
    std::string content;
    content += "{ pkgs, ... }:\n{\n";
    content += "  home.packages = with pkgs; [\n";
    content += join_values(packages, "\n      ");
    content += "\n  ];\n\n";

    content += "  home.programs = [\n";
    content += join_values(programs, "\n    ");
    content += "\n  ];\n}\n";

    try {
        save_to_file(localFile, content);
        return true;
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << '\n';
        return false;
    }
}

void TemplateHandler::loadTemplate() {
    try {
        std::string content = load_from_file(localFile);
        // Parse the file content to extract packages and programs...
    } catch (const std::runtime_error& e) {
        std::cerr << e.what() << '\n';
    }
}

