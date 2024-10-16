#include "../headers/libnix.hpp"
#include <fstream>
#include <sstream>
#include <curl/curl.h>
#include <algorithm>

// Helper to write curl data to file
size_t writeData(void* ptr, size_t size, size_t nmemb, FILE* stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

TemplateHandler::TemplateHandler(const std::string& url, const std::string& localFile)
    : url(url), localFile(localFile) {
    loadTemplate();
}

bool TemplateHandler::downloadTemplate() {
    CURL* curl;
    FILE* fp;
    CURLcode res;
    
    curl = curl_easy_init();
    if (curl) {
        fp = fopen(localFile.c_str(), "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
        return res == CURLE_OK;
    }
    return false;
}

void TemplateHandler::addPackage(const std::string& pkg) {
    packages.push_back(pkg);
    std::sort(packages.begin(), packages.end());
}

void TemplateHandler::addProgram(const std::string& prog) {
    programs.push_back(prog);
    std::sort(programs.begin(), programs.end());
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
        std::cout << pkg << std::endl;
    }
}

void TemplateHandler::listPrograms() const {
    std::cout << "Programs:\n";
    for (const auto& prog : programs) {
        std::cout << prog << std::endl;
    }
}

bool TemplateHandler::save() {
    std::ofstream outFile(localFile);
    if (!outFile.is_open()) return false;

    outFile << "{ pkgs, ... }:\n{\n";
    outFile << "  home.packages = with pkgs; [\n";
    
    // Write all packages
    for (const auto& pkg : packages) {
        outFile << "    " << pkg << "\n";
    }
    outFile << "  ];\n\n";

    outFile << "  home.programs = [\n";
    
    // Write all programs (assuming you want to keep them commented out like in the original)
    for (const auto& prog : programs) {
        outFile << "    # program = {\n";
        outFile << "    #   enable = true;\n";
        outFile << "    #   " << prog << ";\n";
        outFile << "    # };\n";
    }
    outFile << "  ];\n";
    
    outFile << "}\n";
    
    outFile.close();
    return true;
}

void TemplateHandler::loadTemplate() {
    std::ifstream inFile(localFile);
    std::string line;

    bool inPackagesSection = false;
    bool inProgramsSection = false;

    while (std::getline(inFile, line)) {
        // Trim leading/trailing spaces from the line
        line.erase(0, line.find_first_not_of(" \t"));
        line.erase(line.find_last_not_of(" \t") + 1);

        // Check if we are entering the packages section
        if (line.find("home.packages") != std::string::npos) {
            inPackagesSection = true;
            inProgramsSection = false;
            continue;
        }

        // Check if we are entering the programs section
        if (line.find("home.programs") != std::string::npos) {
            inPackagesSection = false;
            inProgramsSection = true;
            continue;
        }

        // If we hit the end of the section
        if (line == "];" || line == "};") {
            inPackagesSection = false;
            inProgramsSection = false;
            continue;
        }

        // Collect packages
        if (inPackagesSection && !line.empty() && line[0] != '{' && line[0] != '}') {
            std::string pkg = line;
            // Remove any trailing commas or brackets
            pkg.erase(std::remove(pkg.begin(), pkg.end(), ','), pkg.end());
            pkg.erase(std::remove(pkg.begin(), pkg.end(), '['), pkg.end());
            pkg.erase(std::remove(pkg.begin(), pkg.end(), ']'), pkg.end());

            // Ignore any line that looks like "home.packages = with pkgs;"
            if (pkg.find("home.packages") == std::string::npos && !pkg.empty()) {
                packages.push_back(pkg);
            }
        }

        // Collect programs
        if (inProgramsSection && line.find("enable") != std::string::npos) {
            std::string prog = line;
            if (!prog.empty()) {
                programs.push_back(prog);
            }
        }
    }
    inFile.close();
}

