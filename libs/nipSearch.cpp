#include "../headers/nipSearch.hpp"
#include "../headers/nipUtils.hpp"
#include <stdexcept>
#include <iostream>

NixAPI::NixAPI() {
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
}

NixAPI::~NixAPI() {
    if (curl) curl_easy_cleanup(curl);
    curl_global_cleanup();
}

bool NixAPI::is_site_up() {
    try {
        std::string response = fetch_url(base_url);
        return !response.empty();
    } catch (const std::runtime_error&) {
        return false;
    }
}

std::vector<std::string> NixAPI::search_packages(const std::string& query) {
    std::string url = base_url + "packages?query=" + query;
    std::string response = fetch_url(url);
    return parse_json_response(response);
}

std::vector<std::string> NixAPI::search_options(const std::string& query) {
    std::string url = base_url + "options?query=" + query;
    std::string response = fetch_url(url);
    return parse_json_response(response);
}

