#pragma once

namespace utils
{
    inline uint8* convertToGray(uint8* image, uint32 width, uint32 height, uint32 channels)
    {
        size_t size = width * height * channels;

        uint8* gray_image = (uint8*) malloc(size);
        if (gray_image == nullptr)
        {
            std::cerr << "Failure to allocate memory for the gray image!" << std::endl;
            return nullptr;
        }

        for (uint8 *p = image, *pg = gray_image; p != image + size; p += channels, pg += channels)
        {
            float average = (*p + *(p + 1) + *(p + 2)) / 3;
            *pg = (uint8) average;
            *(pg + 1) = (uint8) average;
            *(pg + 2) = (uint8) average;
        }

        return gray_image;
    }
}