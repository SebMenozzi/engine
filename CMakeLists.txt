cmake_minimum_required(VERSION 3.15)

project(engine
  LANGUAGES CXX
  VERSION 1.0
)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Build options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

# Put default build target
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "RELEASE as default build target")
    set(CMAKE_BUILD_TYPE "RELEASE")
endif()

add_subdirectory(submodules)

# Sources
set(SRC_CPP
  src/camera/camera.cpp
  src/utils/clock/clock.cpp
  src/utils/phillips/phillips.cpp
  src/input/input.cpp
  src/object/cube/cube.cpp
  src/object/plane/plane.cpp
  src/object/skybox/skybox.cpp
  src/object/sphere/uv_sphere.cpp
  src/object/heightmap/heightmap.cpp
  src/object/heightmap/terrain/terrain.cpp
  src/object/heightmap/ocean/ocean.cpp
  src/object/model/assimp_model.cpp
  src/object/model/tiny_gltf_model.cpp
  src/object/mesh.cpp
  src/scene/scene.cpp
  src/shader/shader.cpp
  src/texture/texture.cpp
  src/texture/cubemap_texture.cpp
)

include_directories(
    src
    src/stb_image
    src/scene
    src/shader
    src/texture
    src/camera
    src/input
    src/material
    src/object
    src/object/cube
    src/object/plane
    src/object/skybox
    src/object/sphere
    src/object/heightmap
    src/object/heightmap/terrain
    src/object/heightmap/ocean
    src/object/model
    src/utils
    src/utils/clock
    src/utils/phillips
)

# Default compilation flags
set(CMAKE_CXX_FLAGS "-pedantic -Wall -Wextra -fsanitize=address -Wno-deprecated-declarations -fPIC")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")

# Debug compilation flags
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -DDEBUG")

# Release compilation flags
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -DRELEASE")

# Exec
add_executable(engine)
target_sources(engine PRIVATE "src/main.cpp" ${SRC_CPP})
target_link_libraries(engine PRIVATE ${OPENGL_LIBRARIES} STATIC_LIBRARIES)
