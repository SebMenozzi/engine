#include "skybox.h"

#ifndef BUFFER_OFFSET
    #define BUFFER_OFFSET(offset) ((char*) 0 + offset)
#endif

namespace mesh
{
    Skybox::Skybox(
        float size, 
        std::vector<std::string> filepaths
    )
    : Cube(size, nullptr, nullptr, nullptr), cubemapTexture_(filepaths) {
        cubemapTexture_.load();
    }

    void Skybox::load() {
        if (glIsBuffer(vboID_) == GL_TRUE)
            glDeleteBuffers(1, &vboID_);

        glGenVertexArrays(1, &vaoID_);
        glBindVertexArray(vaoID_);
            glGenBuffers(1, &vboID_);
            glBindBuffer(GL_ARRAY_BUFFER, vboID_);
                int verticesSize = vertices_.size() * sizeof(float);

                // Allocate to the GPU
                glBufferData(GL_ARRAY_BUFFER, verticesSize, 0, GL_STATIC_DRAW);

                glBufferSubData(GL_ARRAY_BUFFER, 0, verticesSize, &vertices_[0]);

                // Vertices
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
                glEnableVertexAttribArray(0);

            glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Skybox::render() {
        glFrontFace(GL_CW);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture_.getID());

        glBindVertexArray(vaoID_);
        glDrawArrays(GL_TRIANGLES, 0, vertices_.size() / 3);
        glDepthMask(GL_TRUE);
        glBindVertexArray(0);

        glFrontFace(GL_CCW);
    }
}