#pragma once
#include <curl/curl.h>
#include <string>
#include <vector>
#include <json.hpp>

class NixAPI {
public:
    NixAPI();
    ~NixAPI();

    bool is_site_up();  // Check if the site is up
    std::vector<std::string> search_packages(const std::string& query);  // Search for packages
    std::vector<std::string> search_options(const std::string& query);   // Search for options

private:
    std::string fetch(const std::string& url);
    std::vector<std::string> parse_results(const std::string& response);

    const std::string base_url = "https://search.nixos.org/";
    CURL* curl;
};
