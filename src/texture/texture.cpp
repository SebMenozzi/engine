#include "texture.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace texture
{
    Texture::Texture(const char* filepath): 
        id_(0),
        filepath_(filepath)
    {
        data_ = stbi_load(filepath_, &width_, &height_, NULL, 3);
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
        
        GLenum internalFormat = GL_RGB;
        GLenum format = GL_RGB;

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
}