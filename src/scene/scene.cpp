#include "scene.h"

namespace scene
{
    // MARK: - Public

    Scene::Scene():
        window_(1280, 720, "Engine"),
        clock_(),
        isWireframe_(false),
        isFullscreen_(false),
        displayNormals_(false)
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

    void Scene::loadShaders_(
        glm::mat4 projection,
        glm::vec3 sunPosition,
        glm::vec3 cameraPosition
    )
    {
        skyboxShader_ = new shader::Shader("assets/shaders/skybox/skybox.vert", "assets/shaders/skybox/skybox.frag");
        skyboxShader_->load();
        skyboxShader_->use();
        skyboxShader_->setMat4("projection", projection);
        skyboxShader_->setInt("skybox", 0);

        textureShader_ = new shader::Shader("assets/shaders/texture/texture.vert", "assets/shaders/texture/texture.frag");
        textureShader_->load();
        textureShader_->use();
        textureShader_->setVec3("fragmentColor", glm::vec3(1.0f, 0.0f, 0.0f));
        textureShader_->setMat4("projection", projection);
        textureShader_->setInt("diffuseTexture", 0);
        textureShader_->setInt("specularTexture", 1);
        textureShader_->setInt("normalTexture", 2);

        terrainShader_ = new shader::Shader("assets/shaders/terrain/terrain.vert", "assets/shaders/terrain/terrain.frag");
        terrainShader_->load();
        terrainShader_->use();
        terrainShader_->setMat4("projection", projection);
        terrainShader_->setFloat("maxHeight", utils::TERRAIN_MAX_HEIGHT);

        grassShader_ = new shader::Shader("assets/shaders/grass/grass.vert", "assets/shaders/grass/grass.frag", "assets/shaders/grass/grass.geom");
        grassShader_->load();
        grassShader_->use();
        grassShader_->setMat4("projection", projection);
        grassShader_->setInt("grassTexture", 0);
        grassShader_->setFloat("grassMinHeight", utils::TERRAIN_GRASS_MIN_HEIGHT);

        oceanShader_ = new shader::Shader("assets/shaders/ocean/ocean.vert", "assets/shaders/ocean/ocean.frag");
        oceanShader_->load();
        oceanShader_->use();
        oceanShader_->setMat4("projection", projection);
        oceanShader_->setVec3("cameraPosition", cameraPosition);

        normalShader_ = new shader::Shader("assets/shaders/normal/normal.vert", "assets/shaders/normal/normal.frag", "assets/shaders/normal/normal.geom");
        normalShader_->load();
        normalShader_->use();
        normalShader_->setMat4("projection", projection);

        materialShader_ = new shader::Shader("assets/shaders/material/material.vert", "assets/shaders/material/material.frag");
        materialShader_->load();
        materialShader_->use();
        materialShader_->setMat4("projection", projection);
        materialShader_->setVec3("cameraPosition", cameraPosition);
        materialShader_->setVec3("sunPosition", sunPosition);

        depthShader_ = new shader::Shader("assets/shaders/depth/depth.vert", "assets/shaders/depth/depth.frag");
        depthShader_->load();
        depthShader_->use();
        depthShader_->setMat4("projection", projection);

        sandShader_ = new shader::Shader("assets/shaders/sand/sand.vert", "assets/shaders/sand/sand.frag");
        sandShader_->load();
        sandShader_->use();
        sandShader_->setMat4("projection", projection);
        sandShader_->setInt("diffuseTexture", 0);
        sandShader_->setInt("specularTexture", 1);
    }

    void Scene::loadTextures_()
    {
        grassTexture_ = new texture::Texture("assets/textures/grass/grass1.png", "grassTexture");
        grassTexture_->load();

        woodTexture_ = new texture::Texture("assets/textures/wood.png", "woodTexture");
        woodTexture_->load();

        moonTexture_ = new texture::Texture("assets/textures/moon.jpg", "moonTexture");
        moonTexture_->load();

        sandTexture_ = new texture::Texture("assets/textures/sand.jpg", "sandTexture");
        sandTexture_->load();

        noiseTexture_ = new texture::Texture("assets/textures/noise.png", "noiseTexture");
        noiseTexture_->load();
    }

    void Scene::loadObjects_()
    {
        std::vector<const char*> faces {
            "assets/textures/skybox/right.png",
            "assets/textures/skybox/left.png",
            "assets/textures/skybox/top.png",
            "assets/textures/skybox/bottom.png",
            "assets/textures/skybox/front.png",
            "assets/textures/skybox/back.png",
        };
        skybox_ = new object::Skybox(100, faces);
        skybox_->load();

        moon_ = new object::UVSphere(0.1, 20.0, 20.0);
        moon_->addTexture(moonTexture_);
        moon_->load();

        cube_ = new object::Cube(1);
        cube_->addTexture(woodTexture_);
        cube_->load();

        terrain_ = new object::Terrain(
            "assets/textures/terrain.png", 
            utils::TERRAIN_SCALE,
            utils::TERRAIN_MIN_HEIGHT,
            utils::TERRAIN_MAX_HEIGHT
        );
        terrain_->addTexture(grassTexture_);
        terrain_->load();

        sand_ = new object::Plane(100);
        sand_->addTexture(sandTexture_);
        sand_->load();

        ocean_ = new object::Ocean(utils::OCEAN_SIZE, utils::OCEAN_SCALE);
        ocean_->load();

        titanic_ = new object::AssimpModel("assets/models/titanic/titanic.obj");
        titanic_->load();

        shark_ = new object::AssimpModel("assets/models/shark.obj");
        shark_->load();
    }

    void Scene::loop_()
    {
        glm::vec3 sunPosition = glm::vec3(0, 10, 0);

        // Projection matrix
        glm::mat4 cameraProjection = glm::perspective(
            glm::radians(45.0f), // The vertical field of view, in radian: the amount of zoom. Usually between 90° (extra wide) and 30° (quite zoomed in)
            16.0f / 9.0f,  // Aspect Ratio. Depends on the size of your window.
            0.1f, // Near clipping plane. Keep as big as possible, or you'll get precision issues
            500.0f // Far clipping plane. Keep as little as possible.
        );

        // View Matrix
        glm::mat4 cameraView = glm::mat4(1.0f);

        glm::vec3 cameraPosition = glm::vec3(4, 2, 0);
        camera::Camera camera(
            cameraPosition,
            glm::vec3(-4, 2, 4), // Target position
            glm::vec3(0, 1, 0), // Head is up (set to 0,-1,0 to look upside-down)
            0.1,
            0.05
        );

        loadShaders_(cameraProjection, cameraPosition, sunPosition);
        loadTextures_();
        loadObjects_();
        createDepthMap_();

        bool lastXKeyState = false;
        bool lastFKeyState = false;
        bool lastNKeyState = false;
        bool lastCKeyState = false;

        ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        while (!window_.getShouldClose())
        {
            // Render scene to depth cubemap

            glViewport(0, 0, 1024, 1024);
            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFboID_);
                glClear(GL_DEPTH_BUFFER_BIT);

                depthShader_->use();

                render_(false);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);

            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            // Poll for and process events
            glfwPollEvents();

            // Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();

            window_.update();

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

            // Toggle Display normals
            if (!lastNKeyState && window_.isKeyPressed(GLFW_KEY_N))
                displayNormals_ = !displayNormals_;
            lastNKeyState = window_.isKeyPressed(GLFW_KEY_N);

            // Toggle Display Depth
            if (!lastCKeyState && window_.isKeyPressed(GLFW_KEY_C))
                displayDepth_ = !displayDepth_;
            lastCKeyState = window_.isKeyPressed(GLFW_KEY_C);

            camera.move(window_);

            // Update camera view (by reference)
            camera.lookAt(cameraView);
            setCameraView_(textureShader_, cameraView);
            setCameraView_(terrainShader_, cameraView);
            setCameraView_(grassShader_, cameraView);
            setCameraView_(oceanShader_, cameraView);
            setCameraView_(normalShader_, cameraView);
            setCameraView_(materialShader_, cameraView);
            setCameraView_(depthShader_, cameraView);
            setCameraView_(sandShader_, cameraView);
            setCameraView_(skyboxShader_, glm::mat4(glm::mat3(cameraView))); // Removes any translation

            sandShader_->use();
            sandShader_->setInt("time", clock_.getTime());

            {
                static float f = 0.0f;

                ImGui::Begin("Engine"); 

                ImGui::Text("EPITA POGLA project.");

                ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
                ImGui::ColorEdit3("clear color", (float*)&clearColor);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }

            render_(displayNormals_);

            ocean_->updateHeights(clock_.getTime());
            ocean_->load();

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // Swap front and back buffers
            window_.swapBuffers();
        }

        glfwTerminate();
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

    void Scene::renderOcean_()
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

        const auto offset = utils::OCEAN_SCALE * utils::OCEAN_SIZE;

        for (int x = -10; x <= 10; x += 2)
        {
            for (int z = -10; z <= 10; z += 2)
            {
                // center
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(x * offset, utils::OCEAN_HEIGHT, z * offset));

                if (displayDepth_)
                    renderObject_(depthShader_, ocean_, model);
                else
                    renderObject_(oceanShader_, ocean_, model);

                if (displayNormals_)
                    renderObject_(normalShader_, ocean_, model);

                // right
                model = glm::make_mat4(symetryZ);
                model = glm::translate(model, glm::vec3(x * offset, utils::OCEAN_HEIGHT, z * offset));

                if (displayDepth_)
                    renderObject_(depthShader_, ocean_, model);
                else
                    renderObject_(oceanShader_, ocean_, model);

                if (displayNormals_)
                    renderObject_(normalShader_, ocean_, model);

                // up
                model = glm::make_mat4(symetryX);
                model = glm::translate(model, glm::vec3(x * offset, utils::OCEAN_HEIGHT, z * offset));
                
                if (displayDepth_)
                    renderObject_(depthShader_, ocean_, model);
                else
                    renderObject_(oceanShader_, ocean_, model);

                if (displayNormals_)
                    renderObject_(normalShader_, ocean_, model);

                // right and up
                model = glm::make_mat4(symetryXZ);
                model = glm::translate(model, glm::vec3(x * offset, utils::OCEAN_HEIGHT, z * offset));
                
                if (displayDepth_)
                    renderObject_(depthShader_, ocean_, model);
                else
                    renderObject_(oceanShader_, ocean_, model);

                if (displayNormals_)
                    renderObject_(normalShader_, ocean_, model);
            }
        }
    }

    void Scene::render_(bool displayNormals)
    {
        glm::mat4 model = glm::mat4(1.0f);

        // Render Moon
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));

        if (displayDepth_)
            renderObject_(depthShader_, moon_, model);
        else
            renderObject_(textureShader_, moon_, model);

        if (displayNormals)
            renderObject_(normalShader_, moon_, model);

        // Render Cube
        model = glm::mat4(0.05f);
        model = glm::translate(model, glm::vec3(0.0f, 1.0f, 0.0f));

        if (displayDepth_)
            renderObject_(depthShader_, cube_, model);
        else
            renderObject_(textureShader_, cube_, model);

        if (displayNormals)
            renderObject_(normalShader_, cube_, model);

        // Render Terrain
        model = glm::mat4(1.0f);

        if (displayDepth_)
            renderObject_(depthShader_, terrain_, model);
        else
            renderObject_(terrainShader_, terrain_, model);

        if (displayNormals)
            renderObject_(normalShader_, terrain_, model);

        // Render Grass
        model = glm::mat4(1.0f);

        renderObject_(grassShader_, terrain_, model);

        // Render Sand
        model = glm::mat4(1.0f);

        if (displayDepth_)
            renderObject_(depthShader_, sand_, model);
        else
            renderObject_(sandShader_, sand_, model);

        if (displayNormals)
            renderObject_(normalShader_, sand_, model);

        // Render Titanic
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(1.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.01f));

        if (displayDepth_)
            renderObject_(depthShader_, titanic_, model);
        else
            renderObject_(materialShader_, titanic_, model);

        if (displayNormals)
            renderObject_(normalShader_, titanic_, model);

        // Render Shark
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.01f));

        if (displayDepth_)
            renderObject_(depthShader_, shark_, model);
        else
            renderObject_(materialShader_, shark_, model);

        if (displayNormals)
            renderObject_(normalShader_, shark_, model);

        // Render Ocean
        renderOcean_();

        // Render Skybox
        skyboxShader_->use();
        skybox_->render();
    }

    void Scene::createDepthMap_()
    {
        glGenFramebuffers(1, &depthMapFboID_);

        // Create depth cubemap texture
        unsigned int depthCubemapTextureID;
        glGenTextures(1, &depthCubemapTextureID);
            glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemapTextureID);

            for (uint32 i = 0; i < 6; ++i)
                glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            // Attach depth texture
            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFboID_);
            glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemapTextureID, 0);

            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}