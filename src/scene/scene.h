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
#include "shader.h"
#include "types.h"
#include "constants.h"

#include "cube.h"
#include "plane.h"
#include "skybox.h"
#include "uv_sphere.h"
#include "heightmap.h"
#include "model.h"

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
        shader::Shader heightmapShader_;
        shader::Shader grassShader_;
        shader::Shader normalShader_;

        bool isWireframe_;

        void sdlDie_(const char* message);
        void setOpenGLAttributes_();
        mesh::Skybox createSkybox_();
        void setCameraView_(shader::Shader& shader, glm::mat4 cameraView);
    };
}