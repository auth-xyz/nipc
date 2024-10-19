#pragma once
#include <string>
#include <vector>
#include <json.hpp>

size_t writeData(void* ptr, size_t size, size_t nmemb, std::string* data);
std::string fetch_url(const std::string& url);
void save_to_file(const std::string& filename, const std::string& data);
std::string load_from_file(const std::string& filename);
std::string join_values(const std::vector<std::string>& values, const std::string& delimiter = "\n");
std::vector<std::string> parse_json_response(const std::string& response);
