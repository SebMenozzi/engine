#include "scene.h"

Scene::Scene(std::string title, int width, int height) : input()
{
    this->title = title;
    this->width = width;
    this->height = height;

    this->mainContext = 0;
    this->mainWindow = 0;
}

Scene::~Scene()
{
    IMG_Quit();
    // Delete our OpengL context
    SDL_GL_DeleteContext(this->mainContext);
    // Destroy our window
    SDL_DestroyWindow(this->mainWindow);
    // Shutdown SDL2
    SDL_Quit();
}

void Scene::sdlDie(const char* message)
{
    std::cout << message << ": " << SDL_GetError() << std::endl;
    SDL_Quit();
}

void Scene::setOpenGLAttributes()
{
	// Set our OpenGL version.
	// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	// 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

    // Turn on double buffering with a 24 bits
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);

    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
}

bool Scene::init()
{
    // Initialize SDL's Video subsystem
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        sdlDie("Unable to initialize SDL");
        return false;
    }

    IMG_Init(IMG_INIT_PNG);

    setOpenGLAttributes();

    this->mainWindow = SDL_CreateWindow(
        this->title.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        this->width,
        this->height,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (this->mainWindow == 0) {
        sdlDie("Unable to create the window");
        return false;
    }

      // Create our opengl context and attach it to our window
      this->mainContext = SDL_GL_CreateContext(mainWindow);

    if (this->mainContext == 0) {
        sdlDie("Unable to create the OpenGL context");
        return false;
    }

    // Check OpenGL properties
    printf("OpenGL loaded\n");
    printf("Vendor:   %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n", glGetString(GL_VERSION));

    // This makes our buffer swap syncronized with the monitor's vertical refresh
    SDL_GL_SetSwapInterval(1);

    // Depth Buffer activation
    glEnable(GL_DEPTH_TEST);

    // Enable antialiasing
    glEnable(GL_MULTISAMPLE);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;
}

Skybox Scene::createSkybox()
{
    std::vector<std::string> faces {
        "assets/textures/skybox/right.jpg",
        "assets/textures/skybox/left.jpg",
        "assets/textures/skybox/top.jpg",
        "assets/textures/skybox/bottom.jpg",
        "assets/textures/skybox/front.jpg",
        "assets/textures/skybox/back.jpg",
    };

    Skybox skybox(
        100,
        "assets/shaders/skybox/skybox.vert",
        "assets/shaders/skybox/skybox.frag",
        faces
    );

    return skybox;
}

void Scene::loop()
{
    unsigned int frameRate (1000 / 60); // 60 fps
    uint32 startLoop = 0, endLoop = 0, durationLoop = 0;

    glClearColor(0.0, 0.0, 0.0, 0.0); // black background

    auto skybox = createSkybox();
    skybox.load();

    std::vector<DirLight> dirLights;
    std::vector<PointLight> pointLights;
    std::vector<SpotLight> spotLights;

    // Sun light
    DirLight sunLight;
    sunLight.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    sunLight.ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    sunLight.diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
    sunLight.specular = glm::vec3(0.5f, 0.5f, 0.5f);
    dirLights.push_back(sunLight);

    glm::vec3 lightPosition = glm::vec3(8.0f, 6.0f, 8.0f);

    // Blue Light
    PointLight blueLight;
    blueLight.position = lightPosition;
    blueLight.ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    blueLight.diffuse = glm::vec3(0.0f, 0.0f, 1.0f);
    blueLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    pointLights.push_back(blueLight);

    LightCube blueLightCube(
        0.5,
        "assets/shaders/light/light.vert",
        "assets/shaders/light/light.frag",
        blueLight.diffuse
    );
    blueLightCube.load();

    // Red Light
    PointLight redLight;
    redLight.position = lightPosition;
    redLight.ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    redLight.diffuse = glm::vec3(1.0f, 0.0f, 0.0f);
    redLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    pointLights.push_back(redLight);

    LightCube redLightCube(0.5, "assets/shaders/light/light.vert", "assets/shaders/light/light.frag", redLight.diffuse);
    redLightCube.load();

    // Floor
    Floor floor(
        20.0,
        "assets/shaders/texture/texture.vert",
        "assets/shaders/texture/texture.frag",
        "assets/textures/wood.png",
        "assets/textures/wood.png"
    );
    floor.load();
    floor.setPosition(glm::vec3(8.0f, 8.0f, 8.0f));

    // Box
    Box box(
        1.0,
        "assets/shaders/texture/texture.vert",
        "assets/shaders/texture/texture.frag",
        "assets/textures/box2.png",
        "assets/textures/box2_specular.png"
    );
    box.load();
    box.setPosition(glm::vec3(8.0f, 8.0f, 8.0f));

    // Cube
    Cube cube(
        1.0,
        "assets/shaders/mirror/mirror.vert",
        "assets/shaders/mirror/mirror.frag"
    );
    cube.load();
    cube.setPosition(glm::vec3(4.0f, 8.0f, 4.0f));

    UVSphere sphere(
        1.0,
        64,
        64,
        "assets/shaders/mirror/mirror.vert",
        "assets/shaders/mirror/mirror.frag"
    );
    sphere.load();
    sphere.setPosition(glm::vec3(4.0f, 9.0f, 4.0f));

    Chunk chunk(10, 10, "assets/shaders/chunk/chunk.vert", "assets/shaders/chunk/chunk.frag");
    chunk.setPosition(0.0f, 20.0f, 0.0f);
    chunk.fillRandom();
    chunk.load();

    // Projection matrix
    glm::mat4 projection = glm::perspective(
        70.0f,
        (float) this->width / this->height,
        1.0f,
        100.0f
    );

    // Model matrix
    glm::mat4 model = glm::mat4(1.0f);

    // View Matrix
    glm::mat4 view = glm::mat4(1.0f);

    Camera camera(
        glm::vec3(15, 15, 15),
        glm::vec3(0, 0, 0),
        glm::vec3(0, 1, 0),
        0.1,
        0.1
    );

    this->input.displayCursor(false);
    this->input.catchCursor(true);

    while (!this->input.isQuit())
    {

        startLoop = this->clock.getTicks();

        /* Handle events */

        this->input.updateEvents();

        // press escape to quit
        if (this->input.getKey(SDL_SCANCODE_ESCAPE))
            break;

        camera.move(this->input);

        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera.lookAt(view);

        chunk.display(projection, view, model);

        // set light position
        float lightX = sin(this->clock.getTicks() * 0.001);
        float lightY = 2.0f;
        float lightZ = cos(this->clock.getTicks() * 0.001);

        glm::vec3 blueLightTranslation = glm::vec3(
            2.0f * lightX,
            lightY,
            2.0f * lightZ
        );

        glm::vec3 redLightTranslation = glm::vec3(
            3.0f * lightX,
            lightY,
            3.0f * lightZ
        );

        pointLights[0].position = lightPosition + blueLightTranslation;
        pointLights[1].position = lightPosition + redLightTranslation;

        blueLightCube.setPosition(lightPosition + blueLightTranslation);
        redLightCube.setPosition(lightPosition + redLightTranslation);

        blueLightCube.display(projection, view, model);
        redLightCube.display(projection, view, model);

        floor.display(projection, view, model, camera, dirLights, pointLights, spotLights);
        box.display(projection, view, model, camera, dirLights, pointLights, spotLights);

        cube.display(projection, view, model);

        sphere.display(projection, view, model);

        glDepthFunc(GL_LEQUAL);
        view = glm::mat4(glm::mat3(view));
        skybox.display(projection, view, model);
        glDepthFunc(GL_LESS);

        SDL_GL_SwapWindow(this->mainWindow);

        endLoop = this->clock.getTicks();
        durationLoop = endLoop - startLoop;

        // force frame rate to 60fps
        if (durationLoop < frameRate)
            SDL_Delay(frameRate - durationLoop);
    }
}
