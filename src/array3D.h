#pragma once

#include <iostream>
#include <vector>

class Array3D
{
public:
    Array3D(int x, int y, int z): width(x), height(y), data(x * y * z, 0) {}
    int& operator()(int x, int y, int z) {
        return data.at(x + y * width + z * width * height);
    }
private:
    int width, height;
    std::vector<int> data;
};
