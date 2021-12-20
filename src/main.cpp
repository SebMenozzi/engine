#include "scene.h"

// Ensure we are using opengl's core profile only
#define GL3_PROTOTYPES 1

int main(int argc, char *argv[])
{
    scene::Scene scene("My Game Engine", 800, 600);

    if (!scene.init())
        return EXIT_FAILURE;

    scene.loop();

    return EXIT_SUCCESS;
}
