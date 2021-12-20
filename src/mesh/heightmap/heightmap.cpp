#include "heightmap.h"

namespace mesh
{
    Heightmap::Heightmap(
        const char* filepath, 
        float scale, 
        float minHeight, 
        float maxHeight
    ): 
        Mesh(),
        scale_(scale),
        minHeight_(minHeight),
        maxHeight_(maxHeight)
    {
        image_ = IMG_Load(filepath);

        if (image_ == 0)
        {
            std::cerr << SDL_GetError() << std::endl;
            return;
        }

        std::vector<float> tmp;

        for (int y = 0; y < image_->h; y++)
        {
            tmp.clear();

            for (int x = 0; x < image_->w; x++)
            {
                Uint32 pixel = utils::getPixel(image_, x, y);
                Uint8 r = 0, g = 0, b = 0;
                SDL_GetRGB(pixel, image_->format, &r, &g, &b);
                float greyValue = utils::clamp((r + g + b) / 3, 0.0, 255.0);
                float height = utils::scale(greyValue, 0.0, 255.0, minHeight_, maxHeight_);

                tmp.push_back(height);
            }

            heights_.push_back(tmp);
        }

        for (size_t x = 0; x < heights_.size(); ++x)
            for (size_t z = 0; z < heights_[0].size(); ++z)
                addVertices_(x, z);
    }

    float Heightmap::getInterpolatedHeight(float x, float z)
    {
        glm::vec2 current(x, z);

        glm::vec2 neighbor1(std::floor(x), std::floor(z));
        glm::vec2 neighbor2(std::ceil(x), std::floor(z));
        glm::vec2 neighbor3(std::floor(x), std::ceil(z));
        glm::vec2 neighbor4(std::ceil(x), std::ceil(z));
        
        float neighbor1Height = getTileHeight_(neighbor1.x, neighbor1.y);
        float neighbor2Height = getTileHeight_(neighbor2.x, neighbor2.y);
        float neighbor3Height = getTileHeight_(neighbor3.x, neighbor3.y);
        float neighbor4Height = getTileHeight_(neighbor4.x, neighbor4.y);

        // https://math.stackexchange.com/questions/3230376/interpolate-between-4-points-on-a-2d-plane
        float c1 = (1 - x) * (1 - z) * neighbor1Height;
        float c2 = x * (1 - z) * neighbor2Height;
        float c3 = (1 - x) * z * neighbor3Height;
        float c4 = x * z * neighbor4Height;

        return c1 + c2 + c3 + c4;
    }

    // MARK: - Private

    float Heightmap::getTileHeight_(int tileX, int tileZ)
    {
        if (tileX >= image_->w) return 0;
        if (tileZ >= image_->h) return 0;

        if (tileX < 0) return 0;
        if (tileZ < 0) return 0;

        return heights_[tileX][tileZ];
    }

    void Heightmap::addVertices_(float x, float z)
    {
        float h0 = getInterpolatedHeight(x, z);
        float h1 = getInterpolatedHeight(x + 1, z);
        float h2 = getInterpolatedHeight(x, z + 1);
        float h3 = getInterpolatedHeight(x + 1, z + 1);

        // 0
        vertices_.push_back(
            glm::vec3(x * scale_, h0, z * scale_)
        );

        // 1 
        vertices_.push_back(
            glm::vec3((x + 1) * scale_, h1, z * scale_)
        );

        // 2 
        vertices_.push_back(
            glm::vec3(x * scale_, h2, (z + 1) * scale_)
        );

        // 2
        vertices_.push_back(
            glm::vec3(x * scale_, h2, (z + 1) * scale_)
        );

        // 3
        vertices_.push_back(
            glm::vec3((x + 1) * scale_, h3, (z + 1) * scale_)
        );

        // 1 
        vertices_.push_back(
            glm::vec3((x + 1) * scale_, h1, z * scale_)
        );
    }
}
