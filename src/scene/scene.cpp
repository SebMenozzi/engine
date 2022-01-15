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
        window_.setInputMode(GLFW_CURSOR, GLFW_CURSOR_NORMAL);

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
        glDisable(GL_CULL_FACE);
        glCullFace(GL_FRONT);

        // clock wise
        glFrontFace(GL_CW);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
            0.1f, // Near clipping plane. Keep as big as possible, or you'll get precision issues
            500.0f // Far clipping plane. Keep as little as possible.
        );

        // View Matrix
        glm::mat4 cameraView = glm::mat4(1.0f);

        camera::Camera camera(
            glm::vec3(4, 2, 0), // Camera position
            glm::vec3(-4, 2, 4), // Target position
            glm::vec3(0, 1, 0), // Head is up (set to 0,-1,0 to look upside-down)
            0.1,
            0.05
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

        auto sandTexture = new texture::Texture("assets/textures/sand.jpg");
        sandTexture->load();

        // Load objects

        object::UVSphere moon(0.1, 20.0, 20.0);
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

        object::Plane plane(100);
        plane.setDiffuseTexture(sandTexture);
        plane.load();

        terrain.setDiffuseTexture(grassTexture);
        terrain.load();

        object::Ocean ocean(utils::OCEAN_SIZE, utils::OCEAN_SCALE);
        ocean.load();

        bool lastXKeyState = false;
        bool lastFKeyState = false;

        ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        while (!window_.getShouldClose())
        {
            // Poll for and process events
            glfwPollEvents();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

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

            {
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

                ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

                ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

                if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                    counter++;
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            /* START */

            // Render Moon
            model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));
            renderObject_(&basicShader_, &moon, model);
            //renderObject_(&normalShader_, &moon, model);

            // Render Cube
            model = glm::mat4(0.05f);
            model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));
            renderObject_(&basicShader_, &cube, model);
            //renderObject_(&normalShader_, &cube, model);

            // Render Terrain
            model = glm::mat4(1.0f);
            renderObject_(&terrainShader_, &terrain, model);

            // Render Grass
            model = glm::mat4(1.0f);
            renderObject_(&grassShader_, &terrain, model);

            // Render Plane
            model = glm::mat4(1.0f);
            renderObject_(&basicShader_, &plane, model);

            // Render Ocean

            for (int x = -4; x <= 4; ++x)
                for (int z = -4; z <= 4; ++z)
                    renderOcean_(x, z, &oceanShader_, &ocean);

            ocean.updateHeights(clock_.getTime());
            ocean.load();

            // Render Skybox
            skyboxShader_.use();
            skybox.render();

            /* END */

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // Swap front and back buffers
            window_.swapBuffers();
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

    void Scene::renderOcean_(int x, int z, shader::Shader* shader, object::Object* object)
    {
        glm::mat4 model = glm::mat4(1.0f);

        float symetryX[16] = {
            -1, 0,  0,  0,
            0, 1,  0,  0,
            0, 0, 1,  0,
            0, 0,  0,  1
        };

        float symetryZ[16] = {
            1, 0,  0,  0,
            0, 1,  0,  0,
            0, 0, -1,  0,
            0, 0,  0,  1
        };

        float symetryXZ[16] = {
            -1, 0,  0,  0,
            0,  1,  0,  0,
            0,  0, -1,  0,
            0,  0,  0,  1
        };

        float offset = 3 * utils::OCEAN_SIZE * utils::OCEAN_SCALE;

        // ABCD
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(x * offset, utils::OCEAN_HEIGHT, z * offset));
        renderObject_(shader, object, model);
        
        // CBAD bottom

        model = glm::make_mat4(symetryZ);
        model = glm::translate(model, glm::vec3(x * offset, utils::OCEAN_HEIGHT, -z * offset));
        renderObject_(shader, object, model);
        
        // CBAD top
        model = glm::make_mat4(symetryZ);
        model = glm::translate(model, glm::vec3(x * offset, utils::OCEAN_HEIGHT, -z * offset - 2 * utils::OCEAN_SIZE * utils::OCEAN_SCALE));
        renderObject_(shader, object, model);
        
        // ADCB right
        
        model = glm::make_mat4(symetryX);
        model = glm::translate(model, glm::vec3(-x * offset, utils::OCEAN_HEIGHT, z * offset));
        renderObject_(shader, object, model);

        // ADCB left
        
        model = glm::make_mat4(symetryX);
        model = glm::translate(model, glm::vec3(-x * offset - 2 * utils::OCEAN_SIZE * utils::OCEAN_SCALE, utils::OCEAN_HEIGHT, z * offset));
        renderObject_(shader, object, model);

        // CDAB 2
        
        model = glm::make_mat4(symetryX);
        model = glm::translate(model, glm::vec3(-x * offset - 2 * utils::OCEAN_SIZE * utils::OCEAN_SCALE, utils::OCEAN_HEIGHT, z * offset));
        renderObject_(shader, object, model);

        // CDAB bottom right
        model = glm::make_mat4(symetryXZ);
        model = glm::translate(model, glm::vec3(-x * offset, utils::OCEAN_HEIGHT, -z * offset));
        renderObject_(shader, object, model);

        // CDAB bottom left
        model = glm::make_mat4(symetryXZ);
        model = glm::translate(model, glm::vec3(-x * offset - 2 * utils::OCEAN_SIZE * utils::OCEAN_SCALE, utils::OCEAN_HEIGHT, -z * offset));
        renderObject_(shader, object, model);

        // CDAB top right
        model = glm::make_mat4(symetryXZ);
        model = glm::translate(model, glm::vec3(-x * offset, utils::OCEAN_HEIGHT, -z * offset - 2 * utils::OCEAN_SIZE* utils::OCEAN_SCALE));
        renderObject_(shader, object, model);

        // CDAB top left
        model = glm::make_mat4(symetryXZ);
        model = glm::translate(model, glm::vec3(-x * offset - 2 * utils::OCEAN_SIZE * utils::OCEAN_SCALE, utils::OCEAN_HEIGHT, -z * offset - 2 * utils::OCEAN_SIZE * utils::OCEAN_SCALE));
        renderObject_(shader, object, model);
    }
}