#pragma once

#include <glm.hpp>

namespace utils
{
    // Camera
    const float CAMERA_FOV = glm::radians(45.0f);
    const float CAMERA_ASPECT_RATIO = 16.0f / 9.0f;
    const float CAMERA_NEAR_PLANE = 0.1f; // Near clipping plane. Keep as big as possible, or you'll get precision issues
    const float CAMERA_FAR_PLANE = 500.0f; // Far clipping plane. Keep as little as possible.

    // Sun
    const glm::vec3 SUN_DIRECTION = glm::normalize(glm::vec3(1.0f, 0.3f, 1.0f));

    // Depth Map
    const float DEPTH_MAP_RESOLUTION = 4096;

    // Terrain
    const float TERRAIN_MIN_HEIGHT = -0.51;
    const float TERRAIN_MAX_HEIGHT = 0.9;
    const float TERRAIN_SCALE = 0.01;
    const float TERRAIN_GRASS_MIN_HEIGHT = 0.05;

    // Ocean
    const float OCEAN_SIZE = 64.0;
    const float OCEAN_HEIGHT = 0.05;
    const float OCEAN_SCALE = 0.1;
}