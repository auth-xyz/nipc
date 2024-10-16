#include <iostream>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
#include "../headers/libcli.hpp"

CLIParser::CLIParser() {
    add_flag("help", "h", "Display this help message", false);
}

void CLIParser::add_argument(const std::string& name, const std::string& alias, bool required, const std::string& description) {
    arguments_[name] = {alias, required, description, ""};
    if (!alias.empty()) alias_map_[alias] = name;
}

void CLIParser::add_flag(const std::string& name, const std::string& alias, const std::string& description, bool default_value) {
    flags_[name] = {alias, default_value, description, false};
    if (!alias.empty()) alias_map_[alias] = name;
}

void CLIParser::parse(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg.starts_with("--")) arg = arg.substr(2);
        else if (arg.starts_with("-")) arg = alias_map_.count(arg.substr(1)) ? alias_map_[arg.substr(1)] : arg.substr(1);

        if (flags_.count(arg)) {
            flags_[arg].value = true;
        } else if (arguments_.count(arg)) {
            if (i + 1 < argc) arguments_[arg].value = argv[++i];
            else if (arguments_[arg].required) missing_args_.push_back(arg);
        } else {
            std::cerr << "Unknown argument: " << arg << '\n';
            exit(1);
        }
    }

    if (flags_["help"].value) {
        print_help();
        exit(0);
    }

    for (const auto& arg : arguments_) {
        if (arg.second.required && arg.second.value.empty()) {
            std::cerr << "Missing required argument: " << arg.first << '\n';
            missing_args_.push_back(arg.first);
        }
    }

    if (!missing_args_.empty()) {
        print_help();
        exit(1);
    }
}

bool CLIParser::flag(const std::string& name) const {
    return flags_.at(name).value;
}

std::string CLIParser::argument(const std::string& name) const {
    return arguments_.at(name).value;
}

void CLIParser::print_help() const {
    std::cout << "Flags:\n";
    for (const auto& [name, flag] : flags_) {
        std::cout << "  --" << name;
        if (!flag.alias.empty()) std::cout << ", -" << flag.alias;
        std::cout << ": " << flag.description << " (default: " << (flag.default_value ? "true" : "false") << ")\n";
    }

    std::cout << "\nArguments:\n";
    for (const auto& [name, arg] : arguments_) {
        std::cout << "  --" << name;
        if (!arg.alias.empty()) std::cout << ", -" << arg.alias;
        std::cout << ": " << arg.description << (arg.required ? " (required)" : " (optional)") << '\n';
    }
}

