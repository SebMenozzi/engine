#include "scene.h"

namespace scene
{
    // MARK: - Public

    Scene::Scene(
        std::string title, 
        int width, 
        int height
    ): 
        title_(title),
        width_(width),
        height_(height),
        mainWindow_(0),
        mainContext_(0),
        clock_(),
        input_(),
        basicShader_("assets/shaders/basic/basic.vert", "assets/shaders/basic/basic.frag"),
        skyboxShader_("assets/shaders/skybox/skybox.vert", "assets/shaders/skybox/skybox.frag"),
        terrainShader_("assets/shaders/terrain/terrain.vert", "assets/shaders/terrain/terrain.frag"),
        grassShader_("assets/shaders/grass/grass.vert", "assets/shaders/grass/grass.frag", "assets/shaders/grass/grass.geom"),
        oceanShader_("assets/shaders/ocean/ocean.vert", "assets/shaders/ocean/ocean.frag"),
        normalShader_("assets/shaders/normal/normal.vert", "assets/shaders/normal/normal.frag", "assets/shaders/normal/normal.geom"),
        isWireframe_(false)
    {}

    Scene::~Scene()
    {
        IMG_Quit();
        // Delete our OpengL context
        SDL_GL_DeleteContext(mainContext_);
        // Destroy our window
        SDL_DestroyWindow(mainWindow_);
        // Shutdown SDL2
        SDL_Quit();
    }

    bool Scene::init()
    {
        #ifndef __APPLE__
        // Init glew
        if (glewInit())
        {
            std::cerr << "Error while initializing glew" << std::endl;
            return false;
        }
        #endif

        // Initialize SDL's Video subsystem
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
        {
            sdlDie_("Unable to initialize SDL");
            return false;
        }

        IMG_Init(IMG_INIT_PNG);

        setOpenGLAttributes_();

        mainWindow_ = SDL_CreateWindow(
            title_.c_str(),
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            width_,
            height_,
            SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
        );

        if (mainWindow_ == 0)
        {
            sdlDie_("Unable to create the window");
            return false;
        }

        // Create our opengl context and attach it to our window
        mainContext_ = SDL_GL_CreateContext(mainWindow_);

        if (mainContext_ == 0)
        {
            sdlDie_("Unable to create the OpenGL context");
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

        // Enable face culling
        //glDisable(GL_CULL_FACE);
        //glCullFace(GL_FRONT);

        // clock wise
        glFrontFace(GL_CW);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        return true;
    }

    void Scene::loop()
    {
        uint32 frameRate = 1000 / 60; // 60 fps
        uint32 startLoop = 0, endLoop = 0, durationLoop = 0;

        // Create skybox
        object::Skybox skybox = createSkybox_();

        // Projection matrix
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f), // The vertical field of view, in radian: the amount of zoom. Usually between 90° (extra wide) and 30° (quite zoomed in)
            16.0f / 9.0f,  // Aspect Ratio. Depends on the size of your window.
            0.01f, // Near clipping plane. Keep as big as possible, or you'll get precision issues
            1000.0f // Far clipping plane. Keep as little as possible.
        );

        // View Matrix
        glm::mat4 cameraView = glm::mat4(1.0f);

        camera::Camera camera(
            glm::vec3(-5, 5, -5), // Camera position
            glm::vec3(5, 2, 5), // Target position
            glm::vec3(0, 1, 0), // Head is up (set to 0,-1,0 to look upside-down)
            0.1,
            0.1
        );

        // Model matrix
        glm::mat4 model = glm::mat4(1.0f);

        input_.displayCursor(false);
        input_.catchCursor(true);

        // Load shaders

        skyboxShader_.load();
        skyboxShader_.use();
        skyboxShader_.setMat4("projection", projection);
        skyboxShader_.setInt("skybox", 0);

        basicShader_.load();
        basicShader_.use();
        basicShader_.setVec3("fragmentColor", glm::vec3(1.0f, 0.0f, 0.0f));
        basicShader_.setMat4("projection", projection);
        basicShader_.setInt("diffuseTexture", 0);
        basicShader_.setInt("specularTexture", 1);
        basicShader_.setInt("normalTexture", 2);

        terrainShader_.load();
        terrainShader_.use();
        terrainShader_.setMat4("projection", projection);
        terrainShader_.setFloat("maxHeight", utils::TERRAIN_MAX_HEIGHT);

        grassShader_.load();
        grassShader_.use();
        grassShader_.setMat4("projection", projection);
        grassShader_.setInt("grassTexture", 0);
        grassShader_.setFloat("grassMinHeight", utils::TERRAIN_GRASS_MIN_HEIGHT);

        oceanShader_.load();
        oceanShader_.use();
        oceanShader_.setMat4("projection", projection);

        normalShader_.load();
        normalShader_.use();
        normalShader_.setMat4("projection", projection);

        // Load textures

        auto grassTexture = new texture::Texture("assets/textures/grass/grass1.png");
        grassTexture->load();

        auto woodTexture = new texture::Texture("assets/textures/wood.png");
        woodTexture->load();

        auto moonTexture = new texture::Texture("assets/textures/moon.jpg");
        moonTexture->load();

        // Load objects

        object::UVSphere moon(1, 40.0, 40.0);
        moon.setMaterial(new material::Material(
            glm::vec3(0), 
            glm::vec3(1, 0, 0), 
            glm::vec3(0))
        );
        moon.setDiffuseTexture(moonTexture);
        moon.load();

        object::Cube cube(1);
        cube.setDiffuseTexture(woodTexture);
        cube.load();

        object::Terrain terrain(
            "assets/textures/terrain.png", 
            utils::TERRAIN_SCALE,
            utils::TERRAIN_MIN_HEIGHT,
            utils::TERRAIN_MAX_HEIGHT
        );

        terrain.setDiffuseTexture(grassTexture);
        terrain.load();

        object::Ocean ocean(utils::OCEAN_SIZE, utils::OCEAN_HEIGHT);
        ocean.load();

        object::FxGLTFModel zenly("assets/models/zenly.glb");
        zenly.load();

        bool lastXKeyState = false;

        while (!input_.hasQuit())
        {
            startLoop = clock_.getTicks();

            // listen to SDL events
            input_.updateEvents();

            // press escape to quit
            if (input_.getKey(SDL_SCANCODE_ESCAPE))
                break;

            // Toggle Wireframe mode
            if (!lastXKeyState && input_.getKey(SDL_SCANCODE_X))
            {
                isWireframe_ = !isWireframe_;

                if (isWireframe_)
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                else
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            lastXKeyState = input_.getKey(SDL_SCANCODE_X);

            camera.move(input_);

            // Update camera view (by reference)
            camera.lookAt(cameraView);
            setCameraView_(&basicShader_, cameraView);
            setCameraView_(&terrainShader_, cameraView);
            setCameraView_(&grassShader_, cameraView);
            setCameraView_(&oceanShader_, cameraView);
            setCameraView_(&normalShader_, cameraView);
            setCameraView_(&skyboxShader_, glm::mat4(glm::mat3(cameraView))); // Removes any translation

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            /* START */

            // Render Moon
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(5.0f, 15.0f, 4.0f));
            renderObject_(&basicShader_, &moon, model);
            //renderObject_(&normalShader_, &moon, model);

            // Render Cube
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(7.0f, 3.0f, 4.0f));
            renderObject_(&basicShader_, &cube, model);
            //renderObject_(&normalShader_, &cube, model);

            // Render Object
            model = glm::mat4(1.0f);
            renderObject_(&basicShader_, &zenly, model);

            // Render Terrain
            model = glm::mat4(1.0f);
            renderObject_(&terrainShader_, &terrain, model);

            // Render Grass
            model = glm::mat4(1.0f);
            renderObject_(&grassShader_, &terrain, model);

            // Render Ocean
            for (float i = -10; i < 10; ++i)
            {
                for (float j = -10; j < 10; ++j)
                {
                    model = glm::mat4(1.0f);
                    model = glm::translate(model, glm::vec3(i * utils::OCEAN_SIZE, 1.0f, j * utils::OCEAN_SIZE));
                    renderObject_(&oceanShader_, &ocean, model);
                }
            }

            // Render Skybox
            skyboxShader_.use();
            skybox.render();

            /* END */

            SDL_GL_SwapWindow(mainWindow_);

            endLoop = clock_.getTicks();
            durationLoop = endLoop - startLoop;

            // Force frame rate to 60fps
            if (durationLoop < frameRate)
                SDL_Delay(frameRate - durationLoop);
        }
    }

    // MARK: - Private

    void Scene::sdlDie_(const char* message) {
        std::cerr << message << ": " << SDL_GetError() << std::endl;
        SDL_Quit();
    }

    void Scene::setOpenGLAttributes_() {
        // Set our OpenGL version.
        // SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        // OpenGL 4.1
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

        SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

        // Turn on double buffering with a 32 bits
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 32);

        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
        SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);

        SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    }

    object::Skybox Scene::createSkybox_() {
        std::vector<std::string> faces {
            "assets/textures/skybox/right.png",
            "assets/textures/skybox/left.png",
            "assets/textures/skybox/top.png",
            "assets/textures/skybox/bottom.png",
            "assets/textures/skybox/front.png",
            "assets/textures/skybox/back.png",
        };

        object::Skybox skybox(100, faces);
        skybox.load();

        return skybox;
    }

    void Scene::setCameraView_(shader::Shader* shader, glm::mat4 cameraView)
    {
        shader->use();
        shader->setMat4("view", cameraView);
    }

    void Scene::renderObject_(shader::Shader* shader, object::Object* object, glm::mat4 model)
    {
        shader->use();
        shader->setMat4("model", model);
        object->useShader(shader);
        object->render();
    }
}