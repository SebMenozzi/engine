#pragma once

#include <iostream>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "clock.h"
#include "input.h"
#include "camera.h"
#include "texture.h"
#include "shader.h"
#include "types.h"

#include "cube.h"
#include "plane.h"
#include "skybox.h"
#include "uv_sphere.h"

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

        void sdlDie_(const char* message);
        void setOpenGLAttributes_();
        mesh::Skybox createSkybox_();
    };
}