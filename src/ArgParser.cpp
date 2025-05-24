#include "ArgParser.hpp"
#include <iostream>

ArgParser::ArgParser()
{
    flagHandlers["-p"] = flagHandlers["--port"] = [this](std::string value) {
        port = std::stoi(value);
    };

    flagHandlers["-w"] = flagHandlers["--workers"] = [this](std::string value) {
        workers = std::stoi(value);
    };
}

void ArgParser::parse(std::string flag, std::string value)
{
    auto it = flagHandlers.find(flag);
    if (it != flagHandlers.end()) {
        it->second(value);
    } else {
        std::cerr << "Unknown flag: " << flag << std::endl;
        exit(1);
    }
}
