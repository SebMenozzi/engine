#pragma once

#include <iostream>
#include <string>
// OpenGL
#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL/glew.h>
#endif
// GLFW
#include <GLFW/glfw3.h>
// imgui
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
// glm
#include <glm.hpp>
#include <gtx/transform.hpp>
#include <gtc/type_ptr.hpp>

#include "window.h"
#include "clock.h"
#include "camera.h"
#include "texture.h"
#include "material.h"
#include "shader.h"
#include "types.h"
#include "constants.h"

#include "cube.h"
#include "plane.h"
#include "skybox.h"
#include "uv_sphere.h"
#include "terrain.h"
#include "ocean.h"
#include "assimp_model.h"
#include "tiny_gltf_model.h"
#include "object.h"

namespace scene
{
    class Scene
    {
    public:
        Scene();

    private:
        window::Window window_;
        utils::Clock clock_;

        // Shaders
        shader::Shader* textureShader_;
        shader::Shader* skyboxShader_;
        shader::Shader* terrainShader_;
        shader::Shader* grassShader_;
        shader::Shader* oceanShader_;
        shader::Shader* normalShader_;
        shader::Shader* materialShader_;
        shader::Shader* depthShader_;
        shader::Shader* sandShader_;

        // Textures
        texture::Texture* grassTexture_;
        texture::Texture* woodTexture_;
        texture::Texture* moonTexture_;
        texture::Texture* sandTexture_;
        texture::Texture* noiseTexture_;

        // Objects
        object::Skybox* skybox_;
        object::UVSphere* moon_;
        object::Cube* cube_;
        object::Terrain* terrain_;
        object::Plane* sand_;
        object::Ocean* ocean_;
        object::AssimpModel* titanic_;
        object::AssimpModel* shark_;

        GLuint depthMapsFboID_;
        GLuint depthMapsTextureID_;
        std::vector<float> shadowCascadeLevels_;

        bool isWireframe_;
        bool isFullscreen_;
        bool displayNormals_;
        bool displayDepth_;

        bool init_();
        void loadShaders_();
        void loadTextures_();
        void loadObjects_();
        void loop_();
        void setCameraUniformsToShader_(shader::Shader* shader, glm::mat4 cameraView, glm::mat4 cameraProjection, glm::vec3 cameraPosition);
        void renderObject_(shader::Shader* shader, object::Object* object, glm::mat4 model);
        void renderOcean_(bool displayNormals, bool useDepthShader);
        void renderFog_();
        void render_(bool displayNormals, bool useDepthShader);
        void createDepthMapsTexture_();
        std::vector<glm::vec4> getFrustumCornersWorldSpace_(const glm::mat4& projection, const glm::mat4& view);
        glm::mat4 getLightSpaceMatrix_(float nearPlane, float farPlane, glm::mat4 cameraView);
        std::vector<glm::mat4> getLightSpaceMatrices_(glm::mat4 cameraView);
    };
}