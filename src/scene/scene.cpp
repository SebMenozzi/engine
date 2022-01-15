#include "scene.h"

namespace scene
{
    // MARK: - Public

    Scene::Scene():
        window_(1280, 720, "Engine"),
        clock_(),
        basicShader_("assets/shaders/basic/basic.vert", "assets/shaders/basic/basic.frag"),
        skyboxShader_("assets/shaders/skybox/skybox.vert", "assets/shaders/skybox/skybox.frag"),
        terrainShader_("assets/shaders/terrain/terrain.vert", "assets/shaders/terrain/terrain.frag"),
        grassShader_("assets/shaders/grass/grass.vert", "assets/shaders/grass/grass.frag", "assets/shaders/grass/grass.geom"),
        oceanShader_("assets/shaders/ocean/ocean.vert", "assets/shaders/ocean/ocean.frag"),
        normalShader_("assets/shaders/normal/normal.vert", "assets/shaders/normal/normal.frag", "assets/shaders/normal/normal.geom"),
        isWireframe_(false)
    {
        if (!init_())
            exit(EXIT_FAILURE);

        loop_();
    }

    // MARK: - Private

    bool Scene::init_()
    {
        // Init glfw
        if (!glfwInit())
        {
            std::cerr << "Error while initializing GLFW3" << std::endl;
            return false;
        } 

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

        // Setup window
        window_.init();
        window_.setInputMode(GLFW_LOCK_KEY_MODS, GLFW_TRUE);
        window_.setInputMode(GLFW_STICKY_KEYS, GLFW_TRUE);
        window_.setInputMode(GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        // Turn on V-Sync
		glfwSwapInterval(1);

        #ifndef __APPLE__
        // Init glew
        if (glewInit() != GLEW_OK)
        {
            std::cerr << "Error while initializing glew" << std::endl;
            return false;
        }
        #endif

        // Check OpenGL properties
        printf("OpenGL loaded\n");
        printf("Vendor:   %s\n", glGetString(GL_VENDOR));
        printf("Renderer: %s\n", glGetString(GL_RENDERER));
        printf("Version:  %s\n", glGetString(GL_VERSION));

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

        /*
        // Setup Dear ImGui binding
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        ImGui_ImplSDL2_InitForOpenGL(window_, glContext_);
        const char* glslVersion = "#version 130";
        ImGui_ImplOpenGL3_Init(glslVersion);

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        */

        return true;
    }

       void Scene::loop_()
    {
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

        object::TinyGLTFModel zenly("assets/models/zenly.glb");
        zenly.load();

        bool lastXKeyState = false;
        bool lastFKeyState = false;

        while (!window_.getShouldClose())
        {
            window_.update();

            if (window_.getUpdateViewport())
            {
                window_.resize();
                window_.setUpdateViewport(false);
            }

            // Press escape to quit
            if (window_.isKeyPressed(GLFW_KEY_ESCAPE))
                break;

            // Toggle Wireframe mode
            if (!lastXKeyState && window_.isKeyPressed(GLFW_KEY_X))
            {
                isWireframe_ = !isWireframe_;

                if (isWireframe_)
                    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                else
                    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            }
            lastXKeyState = window_.isKeyPressed(GLFW_KEY_X);

            // Toggle Fullscreen
            if (!lastFKeyState && window_.isKeyPressed(GLFW_KEY_F))
            {
                isFullscreen_ = !isFullscreen_;

                window_.setFullscreen(isFullscreen_);
            }
            lastFKeyState = window_.isKeyPressed(GLFW_KEY_F);

            camera.move(window_);

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
            //renderObject_(&grassShader_, &terrain, model);

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

            // Swap front and back buffers
            window_.swapBuffers();
            
            // Poll for and process events
            glfwPollEvents();
        }

        glfwTerminate();
    }
        
    object::Skybox Scene::createSkybox_() {
        std::vector<const char*> faces {
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