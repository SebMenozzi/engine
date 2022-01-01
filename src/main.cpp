#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include "scene.h"
#include "phillips.h"

int main(int argc, char *argv[])
{
    scene::Scene scene("Engine", 1280, 720);

    if (!scene.init())
        return EXIT_FAILURE;

    scene.loop();

    //utils::Phillips phillips(256);

    return EXIT_SUCCESS;
}
