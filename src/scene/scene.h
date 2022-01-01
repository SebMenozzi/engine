#pragma once

#include <iostream>
#include <string>
#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL/glew.h>
#endif
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "clock.h"
#include "input.h"
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
#include "fx_gltf_model.h"
#include "tiny_gltf_model.h"
#include "object.h"

namespace scene
{
    class Scene
    {
    public:
        Scene(std::string title, int width, int height);
        ~Scene();

        bool init();
        void loop();

    private:
        std::string title_;
        int width_;
        int height_;

        SDL_Window* mainWindow_;
        SDL_GLContext mainContext_;

        utils::Clock clock_;
        input::Input input_;

        shader::Shader basicShader_;
        shader::Shader skyboxShader_;
        shader::Shader terrainShader_;
        shader::Shader grassShader_;
        shader::Shader oceanShader_;
        shader::Shader normalShader_;

        bool isWireframe_;

        void sdlDie_(const char* message);
        void setOpenGLAttributes_();
        object::Skybox createSkybox_();
        void setCameraView_(shader::Shader* shader, glm::mat4 cameraView);
        void renderObject_(shader::Shader* shader, object::Object* object, glm::mat4 model);
    };
}