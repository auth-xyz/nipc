#include <string>
#include <vector>
#include <unordered_map>


class CLIParser {
public:
    CLIParser();

    void add_argument(const std::string& name, const std::string& alias = "", bool required = false, const std::string& description = "");
    void add_flag(const std::string& name, const std::string& alias = "", const std::string& description = "", bool default_value = false);
    void parse(int argc, char* argv[]);
    bool flag(const std::string& name) const;
    std::string argument(const std::string& name) const; 
    void print_help() const;

private:
    struct Flag {
        std::string alias;
        bool default_value;
        std::string description;
        bool value;
    };

    struct Argument {
        std::string alias;
        bool required;
        std::string description;
        std::string value;
    };

    std::unordered_map<std::string, Flag> flags_;
    std::unordered_map<std::string, Argument> arguments_;
    std::unordered_map<std::string, std::string> alias_map_;
    std::vector<std::string> missing_args_;
};

