#include "texture.h"

Texture::Texture(const char* imageFilePath)
{
    this->id = 0;
    this->imageFilePath = imageFilePath;
}

Texture::~Texture()
{
    glDeleteTextures(1, &this->id);
}

bool Texture::load()
{
    if(glIsTexture(this->id) == GL_TRUE)
        glDeleteTextures(1, &this->id);

    // Generates the texture id
    glGenTextures(1, &this->id);

    glBindTexture(GL_TEXTURE_2D, this->id);

        SDL_Surface* image = IMG_Load(this->imageFilePath);

        if(image == 0)
        {
            std::cout << "Error : " << SDL_GetError() << std::endl;
            return false;
        }

        SDL_Surface* invertedImage = inverseImage(image);
        SDL_FreeSurface(image);

        GLenum internalFormat = 0;
        GLenum format = 0;

        if (invertedImage->format->BytesPerPixel == 3)
        {
            internalFormat = GL_RGB;

            if(invertedImage->format->Rmask == 0xff)
                format = GL_RGB;
            else
                format = GL_BGR;
        }
        else if (invertedImage->format->BytesPerPixel == 4)
        {
            internalFormat = GL_RGBA;

            if(invertedImage->format->Rmask == 0xff)
                format = GL_RGBA;
            else
                format = GL_BGRA;
        }
        else
        {
            std::cout << "Image internal format not recognized!" << std::endl;
            SDL_FreeSurface(invertedImage);
            return false;
        }

        // Copy of pixels
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, invertedImage->w, invertedImage->h, 0, format, GL_UNSIGNED_BYTE, invertedImage->pixels);

        // Filters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

    SDL_FreeSurface(invertedImage);

    return true;
}

GLuint Texture::getID() const
{
    return this->id;
}
