#include "scene.h"

namespace scene
{
    // MARK: - Public

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

    void Scene::loadShaders_()
    {
        skyboxShader_ = new shader::Shader("assets/shaders/skybox/skybox.vert", "assets/shaders/skybox/skybox.frag");
        skyboxShader_->load();
        skyboxShader_->use();
        skyboxShader_->setInt("skybox", 0);

        textureShader_ = new shader::Shader("assets/shaders/texture/texture.vert", "assets/shaders/texture/texture.frag");
        textureShader_->load();
        textureShader_->use();
        textureShader_->setInt("diffuseTexture", 0);
        textureShader_->setInt("specularTexture", 1);
        textureShader_->setInt("normalTexture", 2);

        terrainShader_ = new shader::Shader("assets/shaders/terrain/terrain.vert", "assets/shaders/terrain/terrain.frag");
        terrainShader_->load();
        terrainShader_->use();
        terrainShader_->setFloat("size", utils::TERRAIN_SIZE);
        terrainShader_->setFloat("scale", utils::TERRAIN_SCALE);
        terrainShader_->setFloat("displacementScale", utils::TERRAIN_DISPLACEMENT_SCALE);
        terrainShader_->setInt("terrainTexture", 1);

        grassShader_ = new shader::Shader("assets/shaders/grass/grass.vert", "assets/shaders/grass/grass.frag", "assets/shaders/grass/grass.geom");
        grassShader_->load();
        grassShader_->use();
        grassShader_->setFloat("size", utils::TERRAIN_SIZE);
        grassShader_->setFloat("scale", utils::TERRAIN_SCALE);
        grassShader_->setFloat("displacementScale", utils::TERRAIN_DISPLACEMENT_SCALE);
        grassShader_->setInt("grassTexture", 0);
        grassShader_->setInt("terrainTexture", 1);
        grassShader_->setFloat("grassMinHeight", utils::TERRAIN_GRASS_MIN_HEIGHT);

        oceanShader_ = new shader::Shader("assets/shaders/ocean/ocean.vert", "assets/shaders/ocean/ocean.frag");
        oceanShader_->load();
        oceanShader_->use();
        oceanShader_->setFloat("size", utils::OCEAN_SIZE);
        oceanShader_->setFloat("scale", utils::OCEAN_SCALE);
        oceanShader_->setFloat("displacementScale", 0.5);
        oceanShader_->setInt("noiseTexture", 0);

        normalShader_ = new shader::Shader("assets/shaders/normal/normal.vert", "assets/shaders/normal/normal.frag", "assets/shaders/normal/normal.geom");
        normalShader_->load();
        normalShader_->use();

        materialShader_ = new shader::Shader("assets/shaders/material/material.vert", "assets/shaders/material/material.frag");
        materialShader_->load();
        materialShader_->use();

        sandShader_ = new shader::Shader("assets/shaders/sand/sand.vert", "assets/shaders/sand/sand.frag");
        sandShader_->load();
        sandShader_->use();
        sandShader_->setInt("sandTexture", 0);
    }

    void Scene::loadTextures_()
    {
        grassTexture_ = new texture::Texture("assets/textures/grass/grass1.png", "grassTexture");
        grassTexture_->load();

        terrainTexture_ = new texture::Texture("assets/textures/terrain.png", "terrainTexture");
        terrainTexture_->load();

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

        terrain_ = new object::Heightmap(utils::TERRAIN_SIZE);
        terrain_->addTexture(grassTexture_);
        terrain_->addTexture(terrainTexture_);
        terrain_->load();

        sand_ = new object::Plane(30);
        sand_->addTexture(sandTexture_);
        sand_->load();

        ocean_ = new object::Heightmap(utils::OCEAN_SIZE);
        ocean_->load();

        shark_ = new object::AssimpModel("assets/models/shark.obj");
        shark_->load();
    }

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

        // Depth Buffer
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

        loadShaders_();
        loadTextures_();
        loadObjects_();

        return true;
    }

    void Scene::loop_()
    {
        glm::mat4 cameraProjection = glm::perspective(
            utils::CAMERA_FOV,
            utils::CAMERA_ASPECT_RATIO,
            utils::CAMERA_NEAR_PLANE, 
            utils::CAMERA_FAR_PLANE
        );
        glm::mat4 cameraView = glm::mat4(1.0f);
        glm::vec3 cameraPosition = glm::vec3(4, 2, 0);
        camera::Camera camera(
            cameraPosition,
            glm::vec3(-4, 2, 4), // Target position
            glm::vec3(0, 1, 0), // Head is up (set to 0,-1,0 to look upside-down)
            0.1,
            0.05
        );

        bool lastXKeyState = false;
        bool lastFKeyState = false;
        bool lastNKeyState = false;

        ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        terrainShader_->use();
        terrainShader_->setVec3("sunDirection", utils::SUN_DIRECTION);

        oceanShader_->use();
        oceanShader_->setVec3("sunDirection", utils::SUN_DIRECTION);

        while (!window_.getShouldClose())
        {
            camera.move(window_);
            camera.lookAt(cameraView);

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

            setCameraUniformsToShader_(textureShader_, cameraView, cameraProjection, cameraPosition);
            setCameraUniformsToShader_(terrainShader_, cameraView, cameraProjection, cameraPosition);
            setCameraUniformsToShader_(grassShader_, cameraView, cameraProjection, cameraPosition);
            setCameraUniformsToShader_(oceanShader_, cameraView, cameraProjection, cameraPosition);
            setCameraUniformsToShader_(normalShader_, cameraView, cameraProjection, cameraPosition);
            setCameraUniformsToShader_(materialShader_, cameraView, cameraProjection, cameraPosition);
            setCameraUniformsToShader_(sandShader_, cameraView, cameraProjection, cameraPosition);
            setCameraUniformsToShader_(skyboxShader_, glm::mat4(glm::mat3(cameraView)), cameraProjection, cameraPosition);

            sandShader_->use();
            sandShader_->setInt("time", clock_.getTime());

            oceanShader_->use();
            oceanShader_->setInt("time", clock_.getTime());

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

            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            // Swap front and back buffers
            window_.swapBuffers();
        }

        glfwTerminate();
    }

    void Scene::setCameraUniformsToShader_(
        shader::Shader* shader, 
        glm::mat4 cameraView, 
        glm::mat4 cameraProjection,
        glm::vec3 cameraPosition
    )
    {
        shader->use();
        
        shader->setMat4("view", cameraView);
        shader->setMat4("projection", cameraProjection);
        shader->setVec3("cameraPosition", cameraPosition);
    }

    void Scene::renderObject_(shader::Shader* shader, object::Object* object, glm::mat4 model)
    {
        shader->use();
        shader->setMat4("model", model);
        object->useShader(shader);
        object->render();
    }

    void Scene::renderOcean_(bool displayNormals)
    {
        glDisable(GL_CULL_FACE);
        
        glm::mat4 model = glm::mat4(1.0f);

        const auto offset = utils::OCEAN_SCALE * utils::OCEAN_SIZE;

        for (int x = -1; x <= 1; x += 2)
        {
            for (int z = -1; z <= 1; z += 2)
            {
                // center
                model = glm::mat4(1.0f);
                model = glm::translate(model, glm::vec3(x * offset, utils::OCEAN_HEIGHT, z * offset));

                renderObject_(oceanShader_, ocean_, model);

                if (displayNormals)
                    renderObject_(normalShader_, ocean_, model);

                // right
                model = glm::make_mat4(symetryZ);
                model = glm::translate(model, glm::vec3(x * offset, utils::OCEAN_HEIGHT, z * offset));

                renderObject_(oceanShader_, ocean_, model);

                if (displayNormals)
                    renderObject_(normalShader_, ocean_, model);

                // up
                model = glm::make_mat4(symetryX);
                model = glm::translate(model, glm::vec3(x * offset, utils::OCEAN_HEIGHT, z * offset));
                
                renderObject_(oceanShader_, ocean_, model);

                if (displayNormals)
                    renderObject_(normalShader_, ocean_, model);

                // right and up
                model = glm::make_mat4(symetryXZ);
                model = glm::translate(model, glm::vec3(x * offset, utils::OCEAN_HEIGHT, z * offset));
                
                renderObject_(oceanShader_, ocean_, model);

                if (displayNormals)
                    renderObject_(normalShader_, ocean_, model);
            }
        }

        glEnable(GL_CULL_FACE);
    }

    void Scene::render_(bool displayNormals)
    {
        glm::mat4 model = glm::mat4(1.0f);

        // Render Moon
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 2.0f, 0.0f));

        renderObject_(textureShader_, moon_, model);

        if (displayNormals)
            renderObject_(normalShader_, moon_, model);

        // Render Terrain
        model = glm::mat4(1.0f);

        renderObject_(terrainShader_, terrain_, model);

        if (displayNormals)
            renderObject_(normalShader_, terrain_, model);

        // Render Grass
        model = glm::mat4(1.0f);

        renderObject_(grassShader_, terrain_, model);

        // Render Sand
        model = glm::mat4(1.0f);
        
        renderObject_(sandShader_, sand_, model);

        if (displayNormals)
            renderObject_(normalShader_, sand_, model);

        // Render Shark
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 1.0f));
        model = glm::scale(model, glm::vec3(0.01f));

        renderObject_(materialShader_, shark_, model);

        if (displayNormals)
            renderObject_(normalShader_, shark_, model);

        // Render Ocean
        renderOcean_(displayNormals);

        // Render Skybox
        skyboxShader_->use();
        skybox_->render();
    }
}