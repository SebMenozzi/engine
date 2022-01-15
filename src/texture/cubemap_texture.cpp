#include "cubemap_texture.h"

#define STB_IMAGE_STATIC
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

namespace texture
{
    CubemapTexture::CubemapTexture(std::vector<const char*> faces):
        id_(0),
        faces_(faces)
    {}

    CubemapTexture::~CubemapTexture()
    {
        glDeleteTextures(1, &id_);
    }

    bool CubemapTexture::load()
    {
        if (glIsTexture(id_) == GL_TRUE)
            glDeleteTextures(1, &id_);

        // Generates the texture id
        glGenTextures(1, &id_);

        glBindTexture(GL_TEXTURE_CUBE_MAP, id_);
            int width, height, channels;

            for (unsigned int i = 0; i < faces_.size(); ++i)
            {
                uint8* data = stbi_load(faces_[i], &width, &height, &channels, 3);

                if (data == nullptr)
                {
                    std::cerr << "Failure to load the texture: " << faces_[i] << std::endl;
                    return false;
                }

                GLenum internalFormat = GL_RGB;
                GLenum format = GL_RGB;

                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, data);

                // Free image
                stbi_image_free(data);
            }

            // Filters
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
        glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

        return true;
    }

    GLuint CubemapTexture::getID() const
    {
        return id_;
    }
}