#include "../../includes/Utils.hpp"

// Utils for printing messages to the console with timestamps and colors
void printColor(const std::string &msg, const std::string &color) {
    std::cout << color 
            << getFormattedTime(std::time(NULL)) 
            << " • " << msg 
            << DEF << std::endl;
}

void printMessage(const std::string& label, const std::string& message,
    const std::string& color, std::ostream& stream) {
    
    stream << color
        << getFormattedTime(std::time(NULL))
        << " • " << label << ": " << message
        << DEF << std::endl;
}

void printWarning(const std::string& msg) {
    printMessage("[WARNING]", msg, BOLDORANGE, std::cerr);
}

void printSuccess(const std::string& msg) {
    printMessage("[SUCCESS]", msg, BOLDGREEN, std::cout);
}

void printInfo(const std::string& msg) {
    printMessage("[INFO]", msg, BOLDCYAN, std::cout);
}

void printData(const std::string& msg) {
    printMessage("[DATA]", msg, ITALICCYAN, std::cout);
}
