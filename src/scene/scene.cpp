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
        skyboxShader_("assets/shaders/skybox/skybox.vert", "assets/shaders/skybox/skybox.frag")
    {}

    Scene::~Scene() {
        IMG_Quit();
        // Delete our OpengL context
        SDL_GL_DeleteContext(mainContext_);
        // Destroy our window
        SDL_DestroyWindow(mainWindow_);
        // Shutdown SDL2
        SDL_Quit();
    }

    bool Scene::init() {
        // Initialize SDL's Video subsystem
        if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
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

        if (mainWindow_ == 0) {
            sdlDie_("Unable to create the window");
            return false;
        }

        // Create our opengl context and attach it to our window
        mainContext_ = SDL_GL_CreateContext(mainWindow_);

        if (mainContext_ == 0) {
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
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        // clock wise
        glFrontFace(GL_CW);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        return true;
    }

    void Scene::loop() {
        uint32 frameRate = 1000 / 60; // 60 fps
        uint32 startLoop = 0, endLoop = 0, durationLoop = 0;

        // create skybox
        mesh::Skybox skybox = createSkybox_();

        // Projection matrix
        glm::mat4 projection = glm::perspective(
            glm::radians(45.0f), // The vertical field of view, in radian: the amount of zoom. Usually between 90° (extra wide) and 30° (quite zoomed in)
            4.0f / 3.0f,  // Aspect Ratio. Depends on the size of your window.
            1.0f, // Near clipping plane. Keep as big as possible, or you'll get precision issues
            100.0f // Far clipping plane. Keep as little as possible.
        );

        // View Matrix
        glm::mat4 cameraView = glm::mat4(1.0f);

        camera::Camera camera(
            glm::vec3(15, 15, 15), // Camera is at (15,15,15), in World Space
            glm::vec3(0, 0, 0), // and looks at the origin
            glm::vec3(0, 1, 0), // Head is up (set to 0,-1,0 to look upside-down)
            0.1,
            0.6
        );

        // Model matrix
        glm::mat4 model = glm::mat4(1.0f);

        input_.displayCursor(false);
        input_.catchCursor(true);

        // load skybox shader
        skyboxShader_.load();
        skyboxShader_.use();
        skyboxShader_.setMat4("projection", projection);
        skyboxShader_.setInt("skybox", 0);

        basicShader_.load();
        basicShader_.use();
        basicShader_.setVec3("fragmentColor", glm::vec3(1.0f, 0.0f, 0.0f));
        basicShader_.setMat4("projection", projection);

        mesh::UVSphere sphere(4.0, 32.0, 32.0, nullptr, nullptr, nullptr);
        sphere.load();

        mesh::Cube cube(4.0, nullptr, nullptr, nullptr);
        cube.load();

        while (!input_.hasQuit())
        {
            startLoop = clock_.getTicks();

            // listen to SDL events
            input_.updateEvents();

            // press escape to quit
            if (input_.getKey(SDL_SCANCODE_ESCAPE))
                break;

            camera.move(input_);

            // Update camera view (by reference)
            camera.lookAt(cameraView);

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            /* START */

            // Render sphere
            basicShader_.use();
            basicShader_.setMat4("view", cameraView);

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(8.0f, 8.0f, 8.0f));
            basicShader_.setMat4("model", model);
            sphere.render();

            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(-8.0f, 8.0f, 8.0f));
            basicShader_.setMat4("model", model);
            cube.render();

            // update skybox view
            skyboxShader_.use();
            skyboxShader_.setMat4("view", glm::mat4(glm::mat3(cameraView))); // removes any translation
            skybox.render();

            /* END */

            SDL_GL_SwapWindow(mainWindow_);

            endLoop = clock_.getTicks();
            durationLoop = endLoop - startLoop;

            // force frame rate to 60fps
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

    mesh::Skybox Scene::createSkybox_() {
        std::vector<std::string> faces {
            "assets/textures/skybox/right.jpg",
            "assets/textures/skybox/left.jpg",
            "assets/textures/skybox/top.jpg",
            "assets/textures/skybox/bottom.jpg",
            "assets/textures/skybox/front.jpg",
            "assets/textures/skybox/back.jpg",
        };

        mesh::Skybox skybox(100, faces);
        skybox.load();

        return skybox;
    }
}