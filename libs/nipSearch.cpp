#include "../headers/nipSearch.hpp"
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
    if (!curl) return false;
    CURLcode res;
    curl_easy_setopt(curl, CURLOPT_URL, base_url.c_str());
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);  // Send HEAD request
    res = curl_easy_perform(curl);
    return (res == CURLE_OK);
}

std::vector<std::string> NixAPI::search_packages(const std::string& query) {
    std::string url = base_url + "packages?query=" + query;
    std::string response = fetch(url);
    return parse_results(response);
}

std::vector<std::string> NixAPI::search_options(const std::string& query) {
    std::string url = base_url + "options?query=" + query;
    std::string response = fetch(url);
    return parse_results(response);
}

std::string NixAPI::fetch(const std::string& url) {
    if (!curl) throw std::runtime_error("CURL initialization failed");

    std::string response_data;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, [](void* contents, size_t size, size_t nmemb, std::string* data) {
        size_t total_size = size * nmemb;
        data->append(static_cast<char*>(contents), total_size);
        return total_size;
    });
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);

    CURLcode res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
        throw std::runtime_error("Failed to fetch data from URL: " + std::string(curl_easy_strerror(res)));
    }

    // Debug: Print the fetched response (to verify content)
    std::cout << "Response data: " << response_data << std::endl;

    if (response_data.empty()) {
        throw std::runtime_error("Received empty response from the server.");
    }

    return response_data;
}

std::vector<std::string> NixAPI::parse_results(const std::string& response) {
    auto json_data = nlohmann::json::parse(response);
    std::vector<std::string> results;
    
    for (const auto& item : json_data["items"]) {
        results.push_back(item["name"].get<std::string>());
    }
    return results;
}

