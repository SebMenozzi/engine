#include "texturedMesh.h"

#ifndef BUFFER_OFFSET
    #define BUFFER_OFFSET(offset) ((char*) 0 + offset)
#endif

TexturedMesh::TexturedMesh(float size, const char* vertexPath, const char* fragmentPath, const char* diffusePath, const char* specularPath)
: Mesh(size, vertexPath, fragmentPath), diffuse(diffusePath), specular(specularPath)
{
    this->diffuse.load();
    this->specular.load();
}

void TexturedMesh::load()
{
    if(glIsBuffer(this->vboID) == GL_TRUE)
        glDeleteBuffers(1, &this->vboID);

    glGenVertexArrays(1, &this->vaoID);
    glBindVertexArray(this->vaoID);
        glGenBuffers(1, &this->vboID);
        glBindBuffer(GL_ARRAY_BUFFER, this->vboID);
            int verticesSize = this->vertices.size() * sizeof(float);
            int normalsSize = this->normals.size() * sizeof(float);
            int textureCoordSize = this->textureCoord.size() * sizeof(float);

            // Allocate to the GPU
            glBufferData(GL_ARRAY_BUFFER, verticesSize + normalsSize + textureCoordSize, 0, GL_STATIC_DRAW);

            glBufferSubData(GL_ARRAY_BUFFER, 0,                          verticesSize, &this->vertices[0]);
            glBufferSubData(GL_ARRAY_BUFFER, verticesSize,               normalsSize,   &this->normals[0]);
            glBufferSubData(GL_ARRAY_BUFFER, verticesSize + normalsSize, textureCoordSize,  &this->textureCoord[0]);

            // vertices
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0));
            glEnableVertexAttribArray(0);

            // normals
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(verticesSize));
            glEnableVertexAttribArray(1);

            // texture coords
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(verticesSize + normalsSize));
            glEnableVertexAttribArray(2);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void TexturedMesh::display(
    glm::mat4 &projection,
    glm::mat4 &view,
    glm::mat4 &model,
    Camera &camera,
    std::vector<DirLight> dirLights,
    std::vector<PointLight> pointLights,
    std::vector<SpotLight> spotLights)
{
    GLuint worldPositionID = glGetUniformLocation(this->shader.getProgramID(), "worldPosition");
    GLuint projectionID = glGetUniformLocation(this->shader.getProgramID(), "projection");
    GLuint viewID = glGetUniformLocation(this->shader.getProgramID(), "view");
    GLuint modelID = glGetUniformLocation(this->shader.getProgramID(), "model");
    GLuint viewPositionID = glGetUniformLocation(this->shader.getProgramID(), "viewPosition");

    glUseProgram(this->shader.getProgramID());

        glUniform3fv(worldPositionID, 1, glm::value_ptr(this->position));
        glUniformMatrix4fv(projectionID, 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(viewID, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(modelID, 1, GL_FALSE, glm::value_ptr(model));
        glUniform3fv(viewPositionID, 1, glm::value_ptr(camera.getPosition()));

        glUniform1i(glGetUniformLocation(this->shader.getProgramID(), "material.diffuse"), 0);
        glUniform1i(glGetUniformLocation(this->shader.getProgramID(), "material.specular"), 1);

        glUniform1f(glGetUniformLocation(this->shader.getProgramID(), "material.shininess"), 32.0f);

        glUniform1f(glGetUniformLocation(this->shader.getProgramID(), "nbDirLights"), dirLights.size());
        glUniform1f(glGetUniformLocation(this->shader.getProgramID(), "nbPointLights"), pointLights.size());
        glUniform1f(glGetUniformLocation(this->shader.getProgramID(), "nbSpotLights"), spotLights.size());

        // for each directional light
        for (int i = 0; i < dirLights.size(); ++i)
        {
            std::string number = std::to_string(i); // convert i to string

            // direction
            glUniform3f(
                glGetUniformLocation(this->shader.getProgramID(), ("dirLights[" + number + "].direction").c_str()),
                dirLights[i].direction.x, dirLights[i].direction.y, dirLights[i].direction.z
            );

            // ambient color
            glUniform3f(
                glGetUniformLocation(this->shader.getProgramID(), ("dirLights[" + number + "].ambient").c_str()),
                dirLights[i].ambient.x, dirLights[i].ambient.y, dirLights[i].ambient.z
            );

            // diffuse color
            glUniform3f(
                glGetUniformLocation(this->shader.getProgramID(), ("dirLights[" + number + "].diffuse").c_str()),
                dirLights[i].diffuse.x, dirLights[i].diffuse.y, dirLights[i].diffuse.z
            );

            // specular
            glUniform3f(
                glGetUniformLocation(this->shader.getProgramID(), ("dirLights[" + number + "].specular").c_str()),
                dirLights[i].specular.x, dirLights[i].specular.y, dirLights[i].specular.z
            );
        }

        // for each point light
        for (int i = 0; i < pointLights.size(); ++i)
        {
            std::string number = std::to_string(i); // convert i to string

            // position
            glUniform3f(
                glGetUniformLocation(this->shader.getProgramID(), ("pointLights[" + number + "].position").c_str()),
                pointLights[i].position.x, pointLights[i].position.y, pointLights[i].position.z
            );

            // ambient color
            glUniform3f(
                glGetUniformLocation(this->shader.getProgramID(), ("pointLights[" + number + "].ambient").c_str()),
                pointLights[i].ambient.x, pointLights[i].ambient.y, pointLights[i].ambient.z
            );

            // diffuse color
            glUniform3f(
                glGetUniformLocation(this->shader.getProgramID(), ("pointLights[" + number + "].diffuse").c_str()),
                pointLights[i].diffuse.x, pointLights[i].diffuse.y, pointLights[i].diffuse.z
            );

            // specular
            glUniform3f(
                glGetUniformLocation(this->shader.getProgramID(), ("pointLights[" + number + "].specular").c_str()),
                1.0f, 1.0f, 1.0f
            );

            // constant
            glUniform1f(
                glGetUniformLocation(this->shader.getProgramID(), ("pointLights[" + number + "].constant").c_str()),
                1.0f
            );

            // linear
            glUniform1f(
                glGetUniformLocation(this->shader.getProgramID(), ("pointLights[" + number + "].linear").c_str()),
                0.09f
            );

            // quadratic
            glUniform1f(
                glGetUniformLocation(this->shader.getProgramID(), ("pointLights[" + number + "].quadratic").c_str()),
                0.032f
            );
        }

        // for each spot light
        for (int i = 0; i < spotLights.size(); ++i)
        {
            std::string number = std::to_string(i); // convert i to string

            // position
            glUniform3f(
                glGetUniformLocation(this->shader.getProgramID(), ("spotLights[" + number + "].position").c_str()),
                spotLights[i].position.x, spotLights[i].position.y, spotLights[i].position.z
            );

            // direction
            glUniform3f(
                glGetUniformLocation(this->shader.getProgramID(), ("spotLights[" + number + "].direction").c_str()),
                spotLights[i].direction.x, spotLights[i].direction.y, spotLights[i].direction.z
            );

            // ambient color
            glUniform3f(
                glGetUniformLocation(this->shader.getProgramID(), ("spotLights[" + number + "].ambient").c_str()),
                spotLights[i].ambient.x, spotLights[i].ambient.y, spotLights[i].ambient.z
            );

            // diffuse color
            glUniform3f(
                glGetUniformLocation(this->shader.getProgramID(), ("spotLights[" + number + "].diffuse").c_str()),
                spotLights[i].diffuse.x, spotLights[i].diffuse.y, spotLights[i].diffuse.z
            );

            // specular
            glUniform3f(
                glGetUniformLocation(this->shader.getProgramID(), ("spotLights[" + number + "].specular").c_str()),
                0.0f, 1.0f, 0.0f
            );

            // constant
            glUniform1f(
                glGetUniformLocation(this->shader.getProgramID(), ("spotLights[" + number + "].constant").c_str()),
                1.0f
            );

            // linear
            glUniform1f(
                glGetUniformLocation(this->shader.getProgramID(), ("spotLights[" + number + "].linear").c_str()),
                0.07f
            );

            // quadratic
            glUniform1f(
                glGetUniformLocation(this->shader.getProgramID(), ("spotLights[" + number + "].quadratic").c_str()),
                0.017f
            );

            // cutOff
            glUniform1f(
                glGetUniformLocation(this->shader.getProgramID(), ("spotLights[" + number + "].cutOff").c_str()),
                glm::cos(glm::radians(7.0f))
            );

            // outerCutOff
            glUniform1f(
                glGetUniformLocation(this->shader.getProgramID(), ("spotLights[" + number + "].outerCutOff").c_str()),
                glm::cos(glm::radians(10.0f))
            );
        }

        // bind diffuse map
        glEnable(GL_FRAMEBUFFER_SRGB);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, this->diffuse.getID());
        glDisable(GL_FRAMEBUFFER_SRGB);
        // bind specular map
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->specular.getID());

        glBindVertexArray(this->vaoID);
        glDrawArrays(GL_TRIANGLES, 0, this->vertices.size() / 3);

    glUseProgram(0);
}
