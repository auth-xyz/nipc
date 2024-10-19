#include <fstream>
#include <sstream>
#include <curl/curl.h>
#include <algorithm>
#include <memory>

#include "../headers/libnix.hpp"

size_t writeData(void* ptr, size_t size, size_t nmemb, FILE* stream) {
  return fwrite(ptr, size, nmemb, stream);
}

TemplateHandler::TemplateHandler(const std::string& url, const std::string& localFile)
    : url(url), localFile(localFile) {
  loadTemplate();
}

bool TemplateHandler::downloadTemplate() {
  CURL* curl = curl_easy_init();
  if (!curl) return false;

  std::unique_ptr<FILE, decltype(&fclose)> fp(fopen(localFile.c_str(), "wb"), fclose);
  if (!fp) return false;

  curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp.get());
  CURLcode res = curl_easy_perform(curl);
  curl_easy_cleanup(curl);

  return res == CURLE_OK;
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
  std::ofstream outFile(localFile);
  if (!outFile.is_open()) return false;

  outFile << "{ pkgs, ... }:\n{\n";
  outFile << "  home.packages = with pkgs; [\n";
  for (const auto& pkg : packages) {
    outFile << "      " << pkg << '\n';
  }
  outFile << "  ];\n\n";

  outFile << "  home.programs = [\n";
  for (const auto& prog : programs) {
    outFile << "    # program = {\n";
    outFile << "    #   enable = true;\n";
    outFile << "    #   " << prog << ";\n";
    outFile << "    # };\n";
  }
  outFile << "  ];\n";
  outFile << "}\n";

  return true;
}

void TemplateHandler::loadTemplate() {
  std::ifstream inFile(localFile);
  if (!inFile.is_open()) return;

  std::string line;
  bool inPackagesSection = false, inProgramsSection = false;

  while (std::getline(inFile, line)) {
    line.erase(0, line.find_first_not_of(" \t"));
    line.erase(line.find_last_not_of(" \t") + 1);

    if (line.find("home.packages") != std::string::npos) {
      inPackagesSection = true;
      inProgramsSection = false;
      continue;
    }
    if (line.find("home.programs") != std::string::npos) {
      inPackagesSection = false;
      inProgramsSection = true;
      continue;
    }
    if (line == "];" || line == "};") {
      inPackagesSection = false;
      inProgramsSection = false;
      continue;
    }

    if (inPackagesSection && !line.empty() && line[0] != '{' && line[0] != '}') {
      std::string pkg = line;
      pkg.erase(std::remove(pkg.begin(), pkg.end(), ','), pkg.end());
      pkg.erase(std::remove(pkg.begin(), pkg.end(), '['), pkg.end());
      pkg.erase(std::remove(pkg.begin(), pkg.end(), ']'), pkg.end());
      if (!pkg.empty()) packages.push_back(pkg);
    }

    if (inProgramsSection && line.find("enable") != std::string::npos) {
      programs.push_back(line);
    }
  }
}

