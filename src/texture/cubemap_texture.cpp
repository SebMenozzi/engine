#include "cubemap_texture.h"
#include "image_utils.h"

namespace texture
{
    CubemapTexture::CubemapTexture(std::vector<std::string> faces):
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
            for (unsigned int i = 0; i < faces_.size(); ++i)
            {
                SDL_Surface* image = IMG_Load(faces_[i].c_str());

                if(image == 0)
                {
                    std::cerr << "Error : " << SDL_GetError() << std::endl;
                    return false;
                }

                GLenum internalFormat = 0;
                GLenum format = 0;

                if(image->format->BytesPerPixel == 3)
                {
                    internalFormat = GL_RGB;

                    if(image->format->Rmask == 0xff)
                        format = GL_RGB;
                    else
                        format = GL_BGR;
                }
                else if(image->format->BytesPerPixel == 4)
                {
                    internalFormat = GL_RGBA;

                    if(image->format->Rmask == 0xff)
                        format = GL_RGBA;
                    else
                        format = GL_BGRA;
                }
                else
                {
                    std::cerr << "Image internal format not recognized!" << std::endl;
                    SDL_FreeSurface(image);
                    return false;
                }

                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, image->w, image->h, 0, format, GL_UNSIGNED_BYTE, image->pixels);

                SDL_FreeSurface(image);
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