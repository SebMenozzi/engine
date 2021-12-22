#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

namespace utils
{
    inline Uint8* pixelRef(SDL_Surface* image, unsigned x, unsigned y)
    {
        int bpp = image->format->BytesPerPixel;

        return (Uint8*)image->pixels + y * image->pitch + x * bpp;
    }

    inline Uint32 getPixel(SDL_Surface* image, unsigned x, unsigned y)
    {
        Uint8* p = pixelRef(image, x, y);
        switch(image->format->BytesPerPixel)
        {
        case 1:
            return *p;
        case 2:
            return *(Uint16 *)p;
        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
                return p[0] << 16 | p[1] << 8 | p[2];
            else
                return p[0] | p[1] << 8 | p[2] << 16;
        case 4:
                return *(Uint32 *)p;
        }

        return 0;
    }

    inline void putPixel(SDL_Surface* image, unsigned x, unsigned y, Uint32 pixel)
    {
        Uint8* p = pixelRef(image, x, y);

        switch(image->format->BytesPerPixel)
        {
            case 1:
                *p = pixel;
            break;
            case 2:
                *(Uint16 *)p = pixel;
            break;
            case 3:
                if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                {
                    p[0] = (pixel >> 16) & 0xff;
                    p[1] = (pixel >> 8) & 0xff;
                    p[2] = pixel & 0xff;
                } 
                else
                {
                    p[0] = pixel & 0xff;
                    p[1] = (pixel >> 8) & 0xff;
                    p[2] = (pixel >> 16) & 0xff;
                }
                break;
            case 4:
                *(Uint32 *)p = pixel;
            break;
        }
    }

    inline SDL_Surface* invertImage(SDL_Surface *image) 
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

        for (int y = 0; y < image->h; ++y)
        {
            for (int x = 0; x < image->w; ++x)
            {
                Uint32 pixel = getPixel(image, x, y);
                putPixel(invertedImage, x, image->h - 1 - y, pixel);
            }
        }

        return invertedImage;
    }
}