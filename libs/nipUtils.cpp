#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <numeric>
#include <curl/curl.h>
#include <json.hpp>
#include "../include/nipUtils.hpp"

size_t writeData(void* ptr, size_t size, size_t nmemb, std::string* data) {
    size_t total_size = size * nmemb;
    data->append(static_cast<char*>(ptr), total_size);
    return total_size;
}

std::string fetch_url(const std::string& url) {
    CURL* curl = curl_easy_init();
    if (!curl) throw std::runtime_error("CURL initialization failed");

    std::string response_data;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
    
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        throw std::runtime_error("Failed to fetch data from URL: " + std::string(curl_easy_strerror(res)));
    }

    curl_easy_cleanup(curl);
    return response_data;
}

void save_to_file(const std::string& filename, const std::string& data) {
    std::ofstream outFile(filename);
    if (!outFile.is_open()) throw std::runtime_error("Failed to open file: " + filename);
    
    outFile << data;
    outFile.close();
}

std::string load_from_file(const std::string& filename) {
    std::ifstream inFile(filename);
    if (!inFile.is_open()) throw std::runtime_error("Failed to open file: " + filename);

    std::ostringstream ss;
    ss << inFile.rdbuf();
    return ss.str();
}

std::string join_values(const std::vector<std::string>& values, const std::string& delimiter) {
    return std::accumulate(values.begin(), values.end(), std::string(),
                           [&](const std::string& a, const std::string& b) {
                               return a.empty() ? b : a + delimiter + b;
                           });
}

std::vector<std::string> parse_json_response(const std::string& response) {
    auto json_data = nlohmann::json::parse(response);
    std::vector<std::string> results;
    
    for (const auto& item : json_data["items"]) {
        results.push_back(item["name"].get<std::string>());
    }
    
    return results;
}

