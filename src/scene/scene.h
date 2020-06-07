#pragma once

#ifdef __APPLE__
    #include <OpenGL/gl3.h>
#else
    #include <GL3/gl.h>
#endif

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include <iostream>
#include <string>
#include "../clock/clock.h"
#include "../input/input.h"
#include "../camera/camera.h"
#include "../texture/texture.h"
#include "../types.h"
// cubes
#include "../models/cube/cube.h"
#include "../models/cube/box.h"
#include "../models/trash/lightCube.h"
// planes
#include "../models/plane/plane.h"
#include "../models/plane/floor.h"
// world
#include "../world/chunk.h"
#include "../world/heightmap.h"

#include "../models/skybox/skybox.h"

#include "../models/sphere/uvSphere.h"

class Scene
{
public:
    Scene(std::string title, int width, int height);
    ~Scene();

    bool init();
    void loop();

private:
    std::string title;
    int width;
    int height;

    SDL_Window* mainWindow;
    SDL_GLContext mainContext;

    Clock clock;
    Input input;

    void sdlDie(const char* message);
    void setOpenGLAttributes();
    Skybox createSkybox();
};
