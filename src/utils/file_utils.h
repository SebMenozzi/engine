#pragma once

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

namespace utils
{
    inline bool getFileContents(const char* filePath, std::string &source)
    {
        std::ifstream file(filePath);

        if (!file.is_open()) {
            std::cerr << "Unable to open file: " << filePath << std::endl;
            return false;
        }

        std::stringstream stream;

        stream << file.rdbuf();

        source = stream.str();
        file.close();

        return true;
    }
}