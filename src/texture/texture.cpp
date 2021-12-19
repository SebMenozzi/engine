#include "texture.h"

namespace texture
{
    Texture::Texture(const char* filePath): 
        id_(0),
        filePath_(filePath)
    {}

    Texture::~Texture()
    {
        if (filePath_ != nullptr)
            glDeleteTextures(1, &id_);
    }

    bool Texture::load()
    {
        if (filePath_ == nullptr)
            return false;
        
        // Delete texture if exists
        if (glIsTexture(id_) == GL_TRUE)
            glDeleteTextures(1, &id_);

        // Generates the texture id
        glGenTextures(1, &id_);

        glBindTexture(GL_TEXTURE_2D, id_);
            SDL_Surface* image = IMG_Load(filePath_);

            if (image == 0)
            {
                std::cerr << "Error : " << SDL_GetError() << std::endl;
                return false;
            }

            SDL_Surface* invertedImage = utils::invertImage(image);
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
                std::cerr << "Image internal format not recognized!" << std::endl;
                SDL_FreeSurface(invertedImage);
                return false;
            }

            // Copy of pixels
            glTexImage2D(
                GL_TEXTURE_2D, 
                0, 
                internalFormat, 
                invertedImage->w, invertedImage->h, 
                0, 
                format, 
                GL_UNSIGNED_BYTE, 
                invertedImage->pixels
            );

            // Filters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);

        SDL_FreeSurface(invertedImage);

        return true;
    }

    GLuint Texture::getID() const
    {
        return id_;
    }
}