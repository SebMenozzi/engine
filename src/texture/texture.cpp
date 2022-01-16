#include "texture.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace texture
{
    Texture::Texture(const char* filepath, const char* name): 
        id_(0),
        filepath_(filepath),
        name_(name)
    {
        data_ = stbi_load(filepath_, &width_, &height_, &nbChannels_, 0);
        if (!data_)
        {
            std::cerr << "Failure to load the texture: " << filepath << std::endl;
            return;
        }
    }

    Texture::Texture(uint8* data, int width, int height):
        data_(data),
        width_(width),
        height_(height)
    {}

    Texture::~Texture()
    {
        glDeleteTextures(1, &id_);
    }

    bool Texture::load()
    {
        if (data_ == nullptr)
            return false;
        
        GLenum format;
        switch (nbChannels_)
        {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
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
                format, 
                width_, height_, 
                0, 
                format, 
                GL_UNSIGNED_BYTE, 
                data_
            );

            // Free image
            stbi_image_free(data_);
            data_ = nullptr;

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

    const char* Texture::getName() const
    {
        return name_;
    }
}