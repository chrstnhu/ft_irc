#include "../../includes/Utils.hpp"

// Get formatted time of localtime
std::string getFormattedTime(std::time_t time) {
    char buffer[100];

    tm *tm_info = std::localtime(&time);
    std::strftime(buffer, sizeof(buffer), "[%H:%M:%S]", tm_info);
    return std::string(buffer);
}

// Convert number into string
std::string numberToString(int number) {
    std::ostringstream oss;
    oss << number;
    return oss.str();
}

std::string numberToString(double number) {
    std::ostringstream oss;
    oss << number;
    return oss.str();
}

std::string numberToString(size_t number) {
    std::ostringstream oss;
    oss << number;
    return oss.str();
}

// Split string with delimiter
std::vector<std::string> split(std::string const& str, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream stream(str);
    while (std::getline(stream, token, delimiter))
        tokens.push_back(token);
    return tokens;
}