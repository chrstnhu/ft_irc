#ifndef UTILS_HPP
# define UTILS_HPP

# include "Includes.hpp"

std::string getFormattedTime(std::time_t time);

void printColor(const std::string &msg, const std::string &color);
void printMessage(const std::string& label, const std::string& message,
    const std::string& color, std::ostream& stream);
void printWarning(const std::string& msg);
void printSuccess(const std::string& msg);
void printInfo(const std::string& msg);
void printData(const std::string& msg);

std::string numberToString(int number);
std::string numberToString(double number);
std::string numberToString(size_t number);

std::vector<std::string> split(std::string const& str, char delimiter);

# endif
