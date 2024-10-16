#pragma once
#include <string>
#include <vector>
#include <iostream>

class TemplateHandler {
public:
    TemplateHandler(const std::string& url, const std::string& localFile);
    bool downloadTemplate(); 
    void addPackage(const std::string& pkg);
    void addProgram(const std::string& prog);
    void removePackage(const std::string& pkg);
    void removeProgram(const std::string& prog);
    void listPackages() const;
    void listPrograms() const;
    bool save();

private:
    std::string url;
    std::string localFile;
    std::vector<std::string> packages;
    std::vector<std::string> programs;

    void loadTemplate();
};

