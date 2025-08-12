#include "../includes/Command.hpp"

Command::Command(std::string name, int requiredParams, bool mustBeLoggedIn) : 
    _name(name), _requiredParams(requiredParams), _mustBeLoggedIn(mustBeLoggedIn) {}

Command::~Command() {}

// Getters
std::string const &Command::getName(void) { 
    return _name; 
}

int const &Command::getRequiredParamsNumber(void) { 
    return _requiredParams; 
}

bool const &Command::getMustBeLoggedIn(void) { 
    return _mustBeLoggedIn; 
}
