#pragma once

#include <iostream>
#include <string>
// GLFW
#include <GLFW/glfw3.h>
// imgui
//#include "imgui_"
//#include "imgui_impl_glfw.h"
//#include "imgui_impl_opengl3.h"
// OpenGL
#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL/glew.h>
#endif
// glm
#include <glm.hpp>
#include <gtx/transform.hpp>

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

        shader::Shader basicShader_;
        shader::Shader skyboxShader_;
        shader::Shader terrainShader_;
        shader::Shader grassShader_;
        shader::Shader oceanShader_;
        shader::Shader normalShader_;

        bool isWireframe_;
        bool isFullscreen_;

        bool init_();
        void loop_();
        object::Skybox createSkybox_();
        void setCameraView_(shader::Shader* shader, glm::mat4 cameraView);
        void renderObject_(shader::Shader* shader, object::Object* object, glm::mat4 model);
    };
}