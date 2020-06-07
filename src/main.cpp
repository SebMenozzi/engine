#include "./scene/scene.h"

int main(int argc, char *argv[])
{
    Scene scene("My Game Engine", 800, 600);

    if(scene.init() == false)
        return EXIT_FAILURE;

    scene.loop();

    return EXIT_SUCCESS;
}
