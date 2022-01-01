cmake_minimum_required(VERSION 3.12)
project(engine)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# Find Boost library
find_package(Boost REQUIRED COMPONENTS system filesystem program_options)

# Find Glew
find_package(GLEW REQUIRED)

# Find OpenGL
find_package(OpenGL REQUIRED)

# Find GLM
find_package(GLM REQUIRED)

# Find SDL2, SDL2_image, SDL2_mixer, SDL2_ttf
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

# Find Assimp
# https://githubmemory.com/repo/ros-planning/geometric_shapes/issues/23
# @sebmenozzi: I spent an our figuring out assimp_LIBRARIES has to be in lowercase...
find_package(assimp REQUIRED)

# Build options
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_INCLUDE_CURRENT_DIR TRUE)

set(TINYGLTF_HEADER_ONLY ON CACHE INTERNAL "" FORCE)
set(TINYGLTF_INSTALL OFF CACHE INTERNAL "" FORCE)

# Put default build target
if (NOT CMAKE_BUILD_TYPE)
    message(STATUS "RELEASE as default build target")
    set(CMAKE_BUILD_TYPE "RELEASE")
endif()

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
  src/object/model/fx_gltf_model.cpp
  src/object/mesh.cpp
  src/scene/scene.cpp
  src/shader/shader.cpp
  src/texture/texture.cpp
  src/texture/cubemap_texture.cpp
)

include_directories(
    src
    src/tiny_gltf
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
    src/nlohmann
    src/gltf
)

# Default compilation flags
set(CMAKE_CXX_FLAGS "-pedantic -Wall -Wextra -fsanitize=address -Wno-deprecated-declarations")

# Debug compilation flags
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -O0 -DDEBUG")

# Release compilation flags
set(CMAKE_CXX_FLAGS_RELEASE "-Ofast -DRELEASE")

# LIBRARIES

if (CMAKE_BUILD_TYPE STREQUAL "RELEASE")
    set(Boost_USE_STATIC_LIBS ON)
endif()

set(BOOST_LIBRARIES Boost::system Boost::filesystem Boost::program_options ${CMAKE_DL_LIBS})

include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLM_INCLUDE_DIRS})
include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_MIXER_INCLUDE_DIR} ${SDL2_TTF_INCLUDE_DIR})
include_directories(${ASSIMP_INCLUDE_DIR})

# Exec
add_executable(engine)
target_sources(engine PRIVATE "src/main.cpp" ${SRC_CPP})
target_link_libraries(engine PRIVATE ${OPENGL_LIBRARIES} ${GLEW_LIBRARIES} ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARIES} ${SDL2_MIXER_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${GLM_LIBRARIES} ${BOOST_LIBRARIES} ${assimp_LIBRARIES})
