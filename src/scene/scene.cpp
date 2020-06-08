#include "scene.h"

Scene::Scene(std::string title, int width, int height)
    : input(),
    shadowShader("assets/shaders/shadow/shadow.vert", "assets/shaders/shadow/shadow.frag"),
    depthShader("assets/shaders/depth/depth.vert", "assets/shaders/depth/depth.frag", "assets/shaders/depth/depth.gs"),
    skyboxShader("assets/shaders/skybox/skybox.vert", "assets/shaders/skybox/skybox.frag")
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
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
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

    if (this->mainContext == 0)
    {
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

    // Enable face culling
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    // clock wisedepthMap
    glFrontFace(GL_CW);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    return true;
}

DirLight Scene::createSunLight()
{
    DirLight sunLight;
    sunLight.direction = glm::vec3(-0.2f, -1.0f, -0.3f);
    sunLight.ambient = glm::vec3(0.05f, 0.05f, 0.05f);
    sunLight.diffuse = glm::vec3(0.4f, 0.4f, 0.4f);
    sunLight.specular = glm::vec3(0.5f, 0.5f, 0.5f);

    return sunLight;
}

PointLight Scene::createPointLight(glm::vec3 position, glm::vec3 color)
{
    PointLight pointLight;
    pointLight.position = position;
    pointLight.ambient = glm::vec3(0.0f, 0.0f, 0.0f);
    pointLight.diffuse = color;
    pointLight.specular = glm::vec3(1.0f, 1.0f, 1.0f);

    return pointLight;
}

void Scene::render(
    glm::mat4 cameraView,
    const Shader &shader,
    Box &box1,
    Box &box2,
    UVSphere &sphere,
    Floor &floor,
    Skybox &skybox
)
{
    glm::vec3 originPosition = glm::vec3(8.0f, 8.0f, 8.0f);

    // Model matrix
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, originPosition);
    shader.setMat4("model", model);
    box1.render();

    float lightX = sin(this->clock.getTicks() * 0.001);
    float lightY = 2.0f;
    float lightZ = cos(this->clock.getTicks() * 0.001);

    glm::vec3 box2Translation = glm::vec3(
        2.0f * lightX,
        lightY,
        2.0f * lightZ
    );

    glm::vec3 sphereTranslation = glm::vec3(
        8.0f * lightX,
        lightY,
        8.0f * lightZ
    );

    glm::vec3 newBox2Position = originPosition + box2Translation;
    glm::vec3 newSpherePosition = originPosition + sphereTranslation;

    model = glm::mat4(1.0f);
    model = glm::translate(model, newBox2Position);
    shader.setMat4("model", model);
    box2.render();

    model = glm::mat4(1.0f);
    model = glm::translate(model, newSpherePosition);
    shader.setMat4("model", model);
    sphere.render();

    model = glm::mat4(1.0f);
    model = glm::translate(model, originPosition);
    shader.setMat4("model", model);
    floor.render();

    this->skyboxShader.use();
    this->skyboxShader.setMat4("view", glm::mat4(glm::mat3(cameraView))); // removes any translation
    skybox.render();
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

    Skybox skybox(100, faces);
    skybox.load();

    return skybox;
}

void Scene::loop()
{
    unsigned int frameRate(1000 / 60); // 60 fps
    uint32 startLoop = 0, endLoop = 0, durationLoop = 0;

    // create skybox
    Skybox skybox = createSkybox();

    DirLight sunLight = this->createSunLight();
    //this->dirLights.push_back(sunLight);

    // Blue Light
    PointLight blueLight = this->createPointLight(glm::vec3(0.0f, 10.0f, 8.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    this->pointLights.push_back(blueLight);
    PointLight redLight = this->createPointLight(glm::vec3(8.0f, 10.0f, 8.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    this->pointLights.push_back(redLight);

    // Floor
    Floor floor(50.0, "assets/textures/wood.png", "assets/textures/wood.png");
    floor.load();

    Box box1(1.0, "assets/textures/box2.png", "assets/textures/box2_specular.png");
    box1.load();

    Box box2(1.0, "assets/textures/box2.png", "assets/textures/box2_specular.png");
    box2.load();

    // Sphere
    UVSphere sphere(4.0, 20.0, 30.0);
    sphere.load();

    // Projection matrix
    glm::mat4 projection = glm::perspective(
        70.0f,
        (float) this->width / this->height,
        1.0f,
        100.0f
    );

    // View Matrix
    glm::mat4 cameraView = glm::mat4(1.0f);

    Camera camera(
        glm::vec3(15, 15, 15), // Camera is at (15,15,15), in World Space
        glm::vec3(0, 0, 0), // and looks at the origin
        glm::vec3(0, 1, 0), // Head is up (set to 0,-1,0 to look upside-down)
        0.1,
        0.1
    );

    this->shadowShader.load();
    this->shadowShader.use();
    this->shadowShader.setMat4("projection", projection);
    this->shadowShader.setInt("diffuseTexture", 0);
    this->shadowShader.setInt("depthMap", 1);

    this->depthShader.load();
    this->depthShader.use();

    this->skyboxShader.load();
    this->skyboxShader.use();
    this->skyboxShader.setMat4("projection", projection);
    this->skyboxShader.setInt("skybox", 0);

    this->input.displayCursor(false);
    this->input.catchCursor(true);

    // configure depth map FBO
    const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    // create depth cubemap texture
    unsigned int depthCubemap;
    glGenTextures(1, &depthCubemap);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
    for (unsigned int i = 0; i < 6; ++i)
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    // attach depth texture as FBO's depth buffer
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    /*
    Shader textureShader("assets/shaders/texture/texture.vert", "assets/shaders/texture/texture.frag");
    textureShader.load();
    textureShader.use();
    textureShader.setMat4("projection", projection);
    textureShader.setInt("material.diffuse", 0);
    textureShader.setInt("material.specular", 1);
    textureShader.setFloat("material.shininess", 32.0f);
    */

    glm::vec3 lightPosition = glm::vec3(8.0f, 20.0f, 8.0f);

    while (!this->input.isQuit())
    {
        startLoop = this->clock.getTicks();

        // listen to SDL events
        this->input.updateEvents();

        // press escape to quit
        if (this->input.getKey(SDL_SCANCODE_ESCAPE))
            break;

        camera.move(this->input);

        // update camera view (by reference)
        camera.lookAt(cameraView);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Create depth cubemap transformation matrices
        float nearPlane = 1.0f;
        float farPlane  = 25.0f;
        glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float) SHADOW_WIDTH / (float) SHADOW_HEIGHT, nearPlane, farPlane);
        std::vector<glm::mat4> shadowTransforms;
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));
        shadowTransforms.push_back(shadowProj * glm::lookAt(lightPosition, lightPosition + glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f)));

        // Render scene to depth cubemap
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glClear(GL_DEPTH_BUFFER_BIT);
            this->depthShader.use();
            for (unsigned int i = 0; i < 6; ++i)
                this->depthShader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
            this->depthShader.setFloat("farPlane", farPlane);
            this->depthShader.setVec3("lightPosition", lightPosition);
            this->render(
                cameraView,
                this->depthShader,
                box1,
                box2,
                sphere,
                floor,
                skybox
            );
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        glViewport(0, 0, this->width * 2, this->height * 2);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->shadowShader.use();
        this->shadowShader.setMat4("view", cameraView);
        this->shadowShader.setVec3("viewPosition", camera.getPosition());
        this->shadowShader.setVec3("lightPosition", lightPosition);
        this->shadowShader.setInt("shadows", true);
        this->shadowShader.setFloat("farPlane", farPlane);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);

        this->render(
            cameraView,
            this->shadowShader,
            box1,
            box2,
            sphere,
            floor,
            skybox
        );

        /*
        textureShader.use();

        textureShader.setInt("nbDirLights", this->dirLights.size());
        textureShader.setInt("nbPointLights", this->pointLights.size());
        textureShader.setInt("nbSpotLights", this->spotLights.size());

        for (int i = 0; i < this->dirLights.size(); ++i)
        {
            std::string number = std::to_string(i); // convert i to string
            textureShader.setVec3("dirLights[" + number + "].direction", this->dirLights[i].direction);
            textureShader.setVec3("dirLights[" + number + "].ambient", this->dirLights[i].ambient);
            textureShader.setVec3("dirLights[" + number + "].diffuse", this->dirLights[i].diffuse);
            textureShader.setVec3("dirLights[" + number + "].specular", this->dirLights[i].specular);
        }

        for (int i = 0; i < this->pointLights.size(); ++i)
        {
            std::string number = std::to_string(i); // convert i to string
            textureShader.setVec3("pointLights[" + number + "].position", this->pointLights[i].position);
            textureShader.setVec3("pointLights[" + number + "].ambient", this->pointLights[i].ambient);
            textureShader.setVec3("pointLights[" + number + "].diffuse", this->pointLights[i].diffuse);
            textureShader.setVec3("pointLights[" + number + "].specular", 1.0, 1.0, 1.0);
            textureShader.setFloat("pointLights[" + number + "].constant", 1.0);
            textureShader.setFloat("pointLights[" + number + "].linear", 0.09f);
            textureShader.setFloat("pointLights[" + number + "].quadratic", 0.032f);
        }

        for (int i = 0; i < this->spotLights.size(); ++i)
        {
            std::string number = std::to_string(i); // convert i to string
            textureShader.setVec3("spotLights[" + number + "].position", this->spotLights[i].position);
            textureShader.setVec3("spotLights[" + number + "].direction", this->spotLights[i].direction);
            textureShader.setVec3("spotLights[" + number + "].ambient", this->spotLights[i].ambient);
            textureShader.setVec3("spotLights[" + number + "].diffuse", this->spotLights[i].diffuse);
            textureShader.setVec3("spotLights[" + number + "].specular", 0.0f, 1.0f, 0.0f);
            textureShader.setFloat("spotLights[" + number + "].constant", 1.0);
            textureShader.setFloat("spotLights[" + number + "].linear", 0.07f);
            textureShader.setFloat("spotLights[" + number + "].quadratic", 0.017f);
            textureShader.setFloat("spotLights[" + number + "].cutOff", glm::cos(glm::radians(7.0f)));
            textureShader.setFloat("spotLights[" + number + "].outerCutOff", glm::cos(glm::radians(10.0f)));
        }

        textureShader.setMat4("view", cameraView);
        textureShader.setVec3("viewPosition", camera.getPosition());
        */

        SDL_GL_SwapWindow(this->mainWindow);

        endLoop = this->clock.getTicks();
        durationLoop = endLoop - startLoop;

        // force frame rate to 60fps
        if (durationLoop < frameRate)
            SDL_Delay(frameRate - durationLoop);
    }
}
