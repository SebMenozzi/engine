#include "fileUtils.h"

#include <iostream>
#include <fstream>
#include <sstream>

bool getFileContents(const char* filePath, std::string &source)
{
    std::ifstream file(filePath);

    if (!file.is_open()) {
        std::cout << "Unable to open file: " << filePath << std::endl;
        return false;
    }

    std::stringstream stream;

    stream << file.rdbuf();

    source = stream.str();
    file.close();

    return true;
}
