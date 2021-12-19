#include "scene.h"

int main(int argc, char *argv[])
{
    scene::Scene scene("My Game Engine", 800, 600);

    if (!scene.init())
        return EXIT_FAILURE;

    scene.loop();

    return EXIT_SUCCESS;
}
