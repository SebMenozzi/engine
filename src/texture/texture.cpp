#include "texture.h"

namespace texture
{
    Texture::Texture(const char* filePath): 
        id_(0),
        filePath_(filePath)
    {
        SDL_Surface* image = IMG_Load(filePath_);

        if (image == 0)
        {
            std::cerr << "Error : " << SDL_GetError() << std::endl;
            return;
        }

        SDL_Surface* invertedImage = utils::invertImage(image);
        image_ = invertedImage;
    }

    Texture::Texture(SDL_Surface* image):
        id_(0),
        image_(image)
    {}

    Texture::~Texture()
    {
        glDeleteTextures(1, &id_);

        SDL_FreeSurface(image_);
        image_ = nullptr;
    }

    bool Texture::load()
    {
        if (image_ == nullptr)
            return false;
        
        GLenum internalFormat = 0;
        GLenum format = 0;

        if (image_->format->BytesPerPixel == 3)
        {
            internalFormat = GL_RGB;
            format = (image_->format->Rmask == 0xff) ? GL_RGB : GL_BGR;
        }
        else if (image_->format->BytesPerPixel == 4)
        {
            internalFormat = GL_RGBA;
            format = (image_->format->Rmask == 0xff) ? GL_RGBA : GL_BGRA;
        }
        else
        {
            std::cerr << "Image internal format not recognized!" << std::endl;
            return false;
        }

        // Delete texture if exists
        if (glIsTexture(id_) == GL_TRUE)
            glDeleteTextures(1, &id_);

        // Generates the texture id
        glGenTextures(1, &id_);

        glBindTexture(GL_TEXTURE_2D, id_);
            // Copy of pixels
            glTexImage2D(
                GL_TEXTURE_2D, 
                0, 
                internalFormat, 
                image_->w, image_->h, 
                0, 
                format, 
                GL_UNSIGNED_BYTE, 
                image_->pixels
            );

            // Filters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glBindTexture(GL_TEXTURE_2D, 0);

        return true;
    }

    GLuint Texture::getID() const
    {
        return id_;
    }
}