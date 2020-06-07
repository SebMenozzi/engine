#include "imageUtils.h"

SDL_Surface* inverseImage(SDL_Surface *image)
{
    SDL_Surface* invertedImage = SDL_CreateRGBSurface(
        0,
        image->w, image->h,
        image->format->BitsPerPixel,
        image->format->Rmask,
        image->format->Gmask,
        image->format->Bmask,
        image->format->Amask
    );

    unsigned char* imagePixels= (unsigned char*) image->pixels;
    unsigned char* invertedImagePixels = (unsigned char*) invertedImage->pixels;

    for(int i = 0; i < image->h; ++i)
        for(int j = 0; j < image->w * image->format->BytesPerPixel; ++j)
            invertedImagePixels[(image->w * image->format->BytesPerPixel * (image->h - 1 - i)) + j] = imagePixels[(image->w * image->format->BytesPerPixel * i) + j];

    return invertedImage;
}
